#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineCrossSessionAsync.generated.h"

/**
 * Struct representing basic information about an online player.
 */
USTRUCT(BlueprintType)
struct FOnlinePlayerInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Player Info")
    FString Nickname;

    UPROPERTY(BlueprintReadOnly, Category = "Player Info")
    FString UniqueId;

    UPROPERTY(BlueprintReadOnly, Category = "Player Info")
    FString AvatarUrl; // Optional, depends on subsystem
};

/**
 * Struct representing session data.
 */
USTRUCT(BlueprintType)
struct FSessionData
{
    GENERATED_BODY()

    FOnlineSessionSearchResult NativeResult;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString SessionId;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString HostName;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 MaxPlayers;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 OpenSlots;
};

// ---------------------------
// 1. Get Online Subsystem
// ---------------------------
UCLASS()
class USubsystemInfoAsync : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnFound;

    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnNotFound;

    /**
     * Gets the current online subsystem (Steam, EOS, etc).
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Subsystem", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Get Online Subsystem"))
    static USubsystemInfoAsync* GetOnlineSubsystem();

    virtual void Activate() override;

private:
    IOnlineSubsystem* Subsystem;
};

// ---------------------------
// 2. Get Local Player Info
// ---------------------------
UCLASS()
class UGetLocalPlayerInfoAsync : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnFailure;

    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnSuccess;

    /**
     * Gets local player nickname, ID and avatar URL (if available).
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Player", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Get Local Player Info"))
    static UGetLocalPlayerInfoAsync* GetLocalPlayerInfo();

    virtual void Activate() override;

    UPROPERTY(BlueprintReadOnly, Category = "Player Info")
    FOnlinePlayerInfo PlayerInfo;
};

// ---------------------------
// 3. Find Sessions (Cross-platform)
// ---------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionsFound, const TArray<FSessionData>&, Results);

UCLASS()
class UFindSessionsAsync : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnSessionsFound OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnFailure;

    /**
     * Finds online sessions available (supports cross-platform if backend supports it).
     * @param MaxResults - Maximum number of sessions to return.
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Sessions", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Find Online Sessions"))
    static UFindSessionsAsync* FindSessions(int32 MaxResults = 50);

    virtual void Activate() override;

private:
    TSharedPtr<FOnlineSessionSearch> SearchSettings;
    void HandleSearchComplete(bool bSuccess);
};

// ---------------------------
// 4. Manage Session (Create/End/Destroy)
// ---------------------------
UCLASS()
class UManageSessionAsync : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnSuccess;

    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnFailure;

    /**
     * Creates a new online session.
     * @param MaxPlayers - Maximum number of players allowed in the session.
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Sessions", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Create Session"))
    static UManageSessionAsync* CreateSession(int32 MaxPlayers);

    /**
     * Ends an active online session.
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Sessions", meta = (BlueprintInternalUseOnly = "true", DisplayName = "End Session"))
    static UManageSessionAsync* EndSession();

    /**
     * Destroys the online session completely.
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Sessions", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Destroy Session"))
    static UManageSessionAsync* DestroySession();

    virtual void Activate() override;

private:
    enum class ESessionAction { Create, End, Destroy } Action;
    int32 MaxPlayers;
    void HandleCallback(FName SessionName, bool bWasSuccessful);
};

// ---------------------------
// 5. Matchmaking System
// ---------------------------
UCLASS()
class UMatchmakingAsync : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnMatchFound;

    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnMatchNotFound;

    /**
     * Attempts to find an existing session; if not found, creates a new one.
     * @param MaxPlayers - Max players for new session if created.
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Matchmaking", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Start Matchmaking"))
    static UMatchmakingAsync* StartMatchmaking(int32 MaxPlayers);

    virtual void Activate() override;
};

// ---------------------------
// 6. Host Migration
// ---------------------------
UCLASS()
class UHostTransferAsync : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnHostTransferred;

    UPROPERTY(BlueprintAssignable)
    FSimpleMulticastDelegate OnNoPlayersLeft;

    /**
     * Handles host migration to a random player if current host leaves.
     * Destroys session if no players are left.
     */
    UFUNCTION(BlueprintCallable, Category = "Online|Sessions", meta = (BlueprintInternalUseOnly = "true", DisplayName = "Handle Host Migration"))
    static UHostTransferAsync* HandleHostExit();

    virtual void Activate() override;
};
