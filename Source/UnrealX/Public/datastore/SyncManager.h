#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "SyncManager.generated.h"

UENUM(BlueprintType)
enum class EConflictResolution : uint8
{
    Merge     UMETA(DisplayName = "Merge"),
    Overwrite UMETA(DisplayName = "Overwrite"),
    Discard   UMETA(DisplayName = "Discard")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSyncEventHandler);

UCLASS()
class YOURPROJECT_API USyncManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Sync Manager")
    static void SyncData(const FPlayerData& Data, bool bForceSync);

    UFUNCTION(BlueprintCallable, Category = "Sync Manager")
    static bool HasPendingSync(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Sync Manager")
    static void ResolveConflicts(int32 PlayerID, EConflictResolution Strategy);

    UFUNCTION(BlueprintCallable, Category = "Sync Manager")
    static void StartBackgroundSync();

    UFUNCTION(BlueprintCallable, Category = "Sync Manager")
    static void EnableDeltaSync(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Sync Manager")
    static void RegisterSyncEventHandler(FSyncEventHandler Handler);
};
