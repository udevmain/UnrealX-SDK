// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "UnrealXConfig.generated.h"

USTRUCT(BlueprintType)
struct FConfigReturn {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Config")
	bool bIsSuccess;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
	FString Result;
};

UCLASS()
class UNREALX_API UUnrealXConfig : public UObject 
{
	GENERATED_BODY()

public:
	static inline FString AppID = "AppID";

	FConfigReturn GetConfigVar(const FString& Key);
};