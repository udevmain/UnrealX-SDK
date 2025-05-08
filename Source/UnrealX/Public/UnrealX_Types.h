// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnrealX_Types.generated.h"

UENUM(BlueprintType)
enum class EUserData : uint8
{
    ID            UMETA(DisplayName = "ID"),
    Email         UMETA(DisplayName = "Email"),
    Username      UMETA(DisplayName = "Username"),
    WalletAddress  UMETA(DisplayName = "WalletAddress")
};

// Here you can add your data to save the player's data. You can see more details here: https://unrealx.space/docs/saveSystem
USTRUCT(BlueprintType)
struct FPlayerData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    FString PlayerID;

    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    int32 Level;

    UPROPERTY(BlueprintReadWrite, Category = "Player Data")
    int32 Experience;

    FPlayerData() : PlayerID("unrealX-ply-"), PlayerName(""), Level(0), Experience(0)
    {
    }
};

class UnrealX_Types
{
public:
	UnrealX_Types();
	~UnrealX_Types();
};
