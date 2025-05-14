#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "UnrealXOnlinePlayerController.generated.h"

UCLASS()
class UNREALX_API AUnrealXOnlinePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"), Category = "UnrealX|Player")
	void GetUserID(UObject* WorldContextObject, FLatentActionInfo LatentInfo, FString& OutUserID, bool& bIsCompleted);
};
