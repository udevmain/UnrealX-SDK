// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EUserData : uint8
{
    ID            UMETA(DisplayName = "ID"),
    Email         UMETA(DisplayName = "Email"),
    Username      UMETA(DisplayName = "Username"),
    WalletAddress  UMETA(DisplayName = "WalletAddress")
};

class UnrealX_Types
{
private:
    const FString appID = "your-app-id-here"; 

public:
    FString GetAppID() { return appID };
	UnrealX_Types();
	~UnrealX_Types();
};
