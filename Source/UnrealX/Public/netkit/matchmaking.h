#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "matchmaking.generated.h"

UCLASS()
class UNREALX_API UMatchmakerClient : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", AdvancedDisplay = "LatentInfo"), Category = "Matchmaking")
    static void FindMatchAsync(const TArray<FString>& Modes, UObject* WorldContextObject, FLatentActionInfo LatentInfo, FString& OutIP);
};