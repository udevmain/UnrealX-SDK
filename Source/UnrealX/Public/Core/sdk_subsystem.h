#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnrealX_Types.h"
#include "Http.h"
#include "sdk_subsystem.generated.h"

UCLASS()
class UnrealX_API Usdk_subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FString GetAppID() { return appID };

private:
	const FString appID = "your-appID-here";
};
