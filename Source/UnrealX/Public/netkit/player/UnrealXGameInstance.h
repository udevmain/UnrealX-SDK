#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UnrealXGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALX_API UUnrealXGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void SetToken(const FString& InToken);
	FString GetToken() const;

	void SetUserID(const FString& InUserID);
	FString GetUserID() const;

protected:
	virtual void Shutdown() override;

private:
	UPROPERTY()
	FString Token;

	UPROPERTY()
	FString UserID;	
};
