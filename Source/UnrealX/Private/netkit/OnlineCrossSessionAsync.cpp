#include "OnlineCrossSessionAsync.h"
#include "OnlineSubsystem.h"
#include "OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"
#include "Interfaces/OnlineFriendsInterface.h"

// ---------------------------
// 1. Get Online Subsystem
// ---------------------------
USubsystemInfoAsync* USubsystemInfoAsync::GetOnlineSubsystem()
{
    USubsystemInfoAsync* Node = NewObject<USubsystemInfoAsync>();
    return Node;
}

void USubsystemInfoAsync::Activate()
{
    Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        OnFound.Broadcast();
    }
    else
    {
        OnNotFound.Broadcast();
    }
}

// ---------------------------
// 2. Get Local Player Info
// ---------------------------
UGetLocalPlayerInfoAsync* UGetLocalPlayerInfoAsync::GetLocalPlayerInfo()
{
    UGetLocalPlayerInfoAsync* Node = NewObject<UGetLocalPlayerInfoAsync>();
    return Node;
}

void UGetLocalPlayerInfoAsync::Activate()
{
    if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        const IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
            if (UserId.IsValid())
            {
                PlayerInfo.UniqueId = UserId->ToString();
                PlayerInfo.Nickname = Identity->GetPlayerNickname(0);
                PlayerInfo.AvatarUrl = TEXT(""); // May require external avatar API
                OnSuccess.Broadcast();
                return;
            }
        }
    }
    OnFailure.Broadcast();
}

// ---------------------------
// 3. Find Sessions
// ---------------------------
UFindSessionsAsync* UFindSessionsAsync::FindSessions(int32 MaxResults)
{
    UFindSessionsAsync* Node = NewObject<UFindSessionsAsync>();
    Node->SearchSettings = MakeShareable(new FOnlineSessionSearch());
    Node->SearchSettings->MaxSearchResults = MaxResults;
    return Node;
}

void UFindSessionsAsync::Activate()
{
    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            Session->FindSessions(0, SearchSettings.ToSharedRef());
            Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UFindSessionsAsync::HandleSearchComplete));
            return;
        }
    }
    OnFailure.Broadcast();
}

void UFindSessionsAsync::HandleSearchComplete(bool bSuccess)
{
    if (!SearchSettings.IsValid())
    {
        OnFailure.Broadcast();
        return;
    }

    TArray<FSessionData> ResultList;
    for (const auto& Result : SearchSettings->SearchResults)
    {
        FSessionData SessionInfo;
        SessionInfo.NativeResult = Result;
        SessionInfo.SessionId = Result.GetSessionIdStr();
        SessionInfo.HostName = Result.Session.OwningUserName;
        SessionInfo.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
        SessionInfo.OpenSlots = Result.Session.NumOpenPublicConnections;
        ResultList.Add(SessionInfo);
    }

    OnSuccess.Broadcast(ResultList);
}

// ---------------------------
// 4. Manage Session
// ---------------------------
UManageSessionAsync* UManageSessionAsync::CreateSession(int32 MaxPlayers)
{
    UManageSessionAsync* Node = NewObject<UManageSessionAsync>();
    Node->Action = ESessionAction::Create;
    Node->MaxPlayers = MaxPlayers;
    return Node;
}

UManageSessionAsync* UManageSessionAsync::EndSession()
{
    UManageSessionAsync* Node = NewObject<UManageSessionAsync>();
    Node->Action = ESessionAction::End;
    return Node;
}

UManageSessionAsync* UManageSessionAsync::DestroySession()
{
    UManageSessionAsync* Node = NewObject<UManageSessionAsync>();
    Node->Action = ESessionAction::Destroy;
    return Node;
}

void UManageSessionAsync::Activate()
{
    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            const FName SessionName = NAME_GameSession;

            switch (Action)
            {
            case ESessionAction::Create:
            {
                FOnlineSessionSettings Settings;
                Settings.bIsLANMatch = false;
                Settings.bUsesPresence = true;
                Settings.NumPublicConnections = MaxPlayers;
                Settings.bAllowJoinInProgress = true;
                Session->CreateSession(0, SessionName, Settings);
                break;
            }
            case ESessionAction::End:
                Session->EndSession(SessionName);
                break;
            case ESessionAction::Destroy:
                Session->DestroySession(SessionName);
                break;
            }

            Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UManageSessionAsync::HandleCallback));
            return;
        }
    }
    OnFailure.Broadcast();
}

void UManageSessionAsync::HandleCallback(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
        OnSuccess.Broadcast();
    else
        OnFailure.Broadcast();
}

// ---------------------------
// 5. Matchmaking
// ---------------------------
UMatchmakingAsync* UMatchmakingAsync::StartMatchmaking(int32 MaxPlayers)
{
    UMatchmakingAsync* Node = NewObject<UMatchmakingAsync>();
    Node->MaxPlayers = MaxPlayers;
    return Node;
}

void UMatchmakingAsync::Activate()
{
    // Simplified matchmaking using FindSessionsAsync and fallback to CreateSessionAsync
    UFindSessionsAsync* FindNode = UFindSessionsAsync::FindSessions(10);
    FindNode->OnSuccess.AddDynamic(this, [this](const TArray<FSessionData>& Sessions)
    {
        if (Sessions.Num() > 0)
        {
            OnMatchFound.Broadcast();
        }
        else
        {
            UManageSessionAsync* CreateNode = UManageSessionAsync::CreateSession(MaxPlayers);
            CreateNode->OnSuccess.AddDynamic(this, &UMatchmakingAsync::OnMatchFound);
            CreateNode->OnFailure.AddDynamic(this, &UMatchmakingAsync::OnMatchNotFound);
            CreateNode->Activate();
        }
    });
    FindNode->OnFailure.AddDynamic(this, &UMatchmakingAsync::OnMatchNotFound);
    FindNode->Activate();
}

// ---------------------------
// 6. Host Transfer
// ---------------------------
UHostTransferAsync* UHostTransferAsync::HandleHostExit()
{
    return NewObject<UHostTransferAsync>();
}

void UHostTransferAsync::Activate()
{
    // NOTE: This logic would normally require replication tracking.
    // Placeholder logic for demonstration:
    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            // Check if players remain in session
            FNamedOnlineSession* CurrentSession = Session->GetNamedSession(NAME_GameSession);
            if (CurrentSession && CurrentSession->RegisteredPlayers.Num() > 1)
            {
                OnHostTransferred.Broadcast();
            }
            else
            {
                Session->DestroySession(NAME_GameSession);
                OnNoPlayersLeft.Broadcast();
            }
        }
    }
}
