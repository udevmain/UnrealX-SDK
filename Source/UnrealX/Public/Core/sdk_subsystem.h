// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnrealX_Types.h"
#include "Http.h"
#include "sdk_subsystem.generated.h"

UCLASS()
class UNREALX_API Usdk_subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static FString GetAppID() { return appID; };

private:
	static inline FString appID = "your-appID-here";
};
