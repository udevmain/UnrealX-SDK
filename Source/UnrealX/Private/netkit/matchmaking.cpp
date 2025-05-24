#include "netkit/matchmaking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Kismet/GameplayStatics.h"
#include "LatentActions.h"

class FFindMatchLatentAction : public FPendingLatentAction
{
public:
    FString& OutIP;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    TArray<FString> Modes;
    UObject* WorldContextObject;
    int32 AttemptIndex = 0;
    const int32 MaxAttempts = 5;
    const int32 PingThresholds[5] = { 40, 80, 120, 160, 200 };
    FTimerHandle RetryHandle;

    FFindMatchLatentAction(FString& OutResult, const FLatentActionInfo& LatentInfo, const TArray<FString>& InModes, UObject* InWorldContext)
        : OutIP(OutResult)
        , ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
        , Modes(InModes)
        , WorldContextObject(InWorldContext)
    {
        TryFindMatch();
    }

    void TryFindMatch()
    {
        int32 Ping = AttemptIndex < MaxAttempts ? PingThresholds[AttemptIndex] : 999;

        FString ModeStr = FString::Join(Modes, TEXT(","));
        FString Request = FString::Printf(TEXT("FIND:%s;PING:%d"), *ModeStr, Ping);

        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
        TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
        bool bIsValid;
        Addr->SetIp(TEXT("127.0.0.1"), bIsValid);
        Addr->SetPort(9090);

        FSocket* Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("MatchSocket"), false);

        if (!Socket->Connect(*Addr))
        {
            Cleanup(Socket);
            Retry();
            return;
        }

        TCHAR* SerializedChar = Request.GetCharArray().GetData();
        int32 Size = FCString::Strlen(SerializedChar);
        int32 Sent = 0;
        Socket->Send((uint8*)TCHAR_TO_UTF8(SerializedChar), Size, Sent);

        uint8 Response[1024];
        int32 Read = 0;
        Socket->Recv(Response, sizeof(Response), Read);

        FString Result = FString(UTF8_TO_TCHAR((const char*)Response)).Left(Read);
        Cleanup(Socket);

        if (Result != TEXT("NONE"))
        {
            OutIP = Result;
            Done();
        }
        else
        {
            Retry();
        }
    }

    void Retry()
    {
        if (++AttemptIndex > MaxAttempts)
        {
            OutIP = TEXT("NONE");
            Done();
            return;
        }

        if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
        {
            World->GetTimerManager().SetTimer(RetryHandle, [this]() { TryFindMatch(); }, 1.0f, false);
        }
    }

    void Cleanup(FSocket* Socket)
    {
        if (Socket)
        {
            Socket->Close();
            ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        }
    }

    void Done()
    {
        if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject))
        {
            World->GetLatentActionManager().RemoveActionsForObject(CallbackTarget.Get());
        }
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        Response.FinishAndTriggerIf(OutIP != TEXT(""), ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UMatchmakerClient::FindMatchAsync(const TArray<FString>& Modes, UObject* WorldContextObject, FLatentActionInfo LatentInfo, FString& OutIP)
{
    if (FLatentActionManager* Manager = &WorldContextObject->GetWorld()->GetLatentActionManager())
    {
        if (Manager->FindExistingAction<FFindMatchLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
        {
            OutIP = TEXT("");
            Manager->AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FFindMatchLatentAction(OutIP, LatentInfo, Modes, WorldContextObject));
        }
    }
}