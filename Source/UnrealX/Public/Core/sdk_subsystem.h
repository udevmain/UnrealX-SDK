#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnrealX_Types.h"
#include "Http.h"
#include "sdk_subsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserDataReceived, const FString&, UserData);

UCLASS()
class UnrealX_API Usdk_subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnUserDataReceived OnUserDataReceived;

	UFUNCTION(BlueprintCallable, Category = "User Data")
	FString GetUserData(EUserData Type);

private:
	const FString ApiKey = "your-api-here";
	const FName GenDBName = "your-generated-database-name-here";
	const FName AppName = "your-app-name-here";
};
