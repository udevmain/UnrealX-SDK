#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"
#include "CreateUserSessionAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUserSessionCreated, const FString&, Token, const FString&, UserID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUserSessionFailed);

UCLASS()
class UNREALX_API UCreateUserSessionAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUserSessionCreated OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnUserSessionFailed OnFail;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UCreateUserSessionAsync* CreateUserSession(const FString& AppID, const FString& Platform, const FString& PlatformID);

	void Activate() override;

private:
	FString AppID;
	FString Platform;
	FString PlatformID;

	void HandleResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);	
};
