#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Http.h"
#include "community.generated.h"

USTRUCT(BlueprintType)
struct FGameUserData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 ID;

    UPROPERTY(BlueprintReadOnly)
    FString UserID;

    UPROPERTY(BlueprintReadOnly)
    FString ExtraData;

    UPROPERTY(BlueprintReadOnly)
    FString CreatedAt;
};

UCLASS()
class UNREALX_API UCommunity : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", AdvancedDisplay = "LatentInfo"), Category = "UserData")
    static void GetUserData(const FString& AppID, const FString& Platform, const FString& PlatformID, const FLatentActionInfo LatentInfo, FGameUserData& OutData);

    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", AdvancedDisplay = "LatentInfo"), Category = "UserData")
    static void GetExtraDataValue(const FString& UserID, const FString& Identifier, const FLatentActionInfo LatentInfo, FString& OutValue);

    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", AdvancedDisplay = "LatentInfo"), Category = "UserData")
    static void CheckUserExists(const FString& AppID, const FString& Platform, const FString& PlatformID, const FLatentActionInfo LatentInfo, bool& bExists, FString& OutDebugInfo);

    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", AdvancedDisplay = "LatentInfo"), Category = "UserData")
    static void AddUser(const FString& AppID, const FString& Platform, const FString& PlatformID, const FString& ExtraDataJson, const FLatentActionInfo LatentInfo, bool& bSuccess, FString& OutUserID, FString& OutResponse);

    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", AdvancedDisplay = "LatentInfo"), Category = "UserData")
    static void UpdateUserExtraData(const FString& UserID, const FString& ExtraDataJson, const FLatentActionInfo LatentInfo, bool& bSuccess);
};