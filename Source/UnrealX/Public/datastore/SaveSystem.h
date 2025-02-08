#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "SaveSystem.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Save System")
    int32 PlayerID;

    UPROPERTY(BlueprintReadWrite, Category = "Save System")
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite, Category = "Save System")
    int32 Level;

    UPROPERTY(BlueprintReadWrite, Category = "Save System")
    int32 Experience;
};

UCLASS()
class UnrealX_API USaveSystem : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Save System")
    static bool SavePlayerData(int32 PlayerID, const FPlayerData& Data, bool bAutoSync = true);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    static FPlayerData LoadPlayerData(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    static bool DeletePlayerData(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    static bool ExportSaveData(int32 PlayerID, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    static bool ImportSaveData(int32 PlayerID, const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    static bool RollbackPlayerData(int32 PlayerID, int32 VersionID);
};