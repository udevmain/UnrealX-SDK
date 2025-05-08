// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UnrealX_Types.h"
#include "SaveSystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSaveComplete, bool, bSuccess, FString, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnLoadComplete, bool, bSuccess, FString, ErrorMessage, FPlayerData, PlayerData);

UCLASS()
class UNREALX_API USaveSystem : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Save System")
    void SavePlayerData(const FPlayerData& Data, FOnSaveComplete OnComplete);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    void LoadPlayerData(FString PlayerID, FOnLoadComplete Callback);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool DeletePlayerData(FString PlayerID);

private:
    static void OnSaveResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FOnSaveComplete Callback);
    static void OnLoadResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FOnLoadComplete Callback);
};