#include "netkit/player/UnrealXGameInstance.h"

void UUnrealXGameInstance::SetToken(const FString& InToken)
{
	Token = InToken;
}

FString UUnrealXGameInstance::GetToken() const
{
	return Token;
}

void UUnrealXGameInstance::SetUserID(const FString& InUserID)
{
	UserID = InUserID;
}

FString UUnrealXGameInstance::GetUserID() const
{
	return UserID;
}

void UUnrealXGameInstance::Shutdown()
{
	return;
}


