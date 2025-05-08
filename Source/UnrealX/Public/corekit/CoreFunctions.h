// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "CoreFunctions.generated.h"

UCLASS(Blueprintable)
class UNREALX_API UCoreFunctions : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Core|Settings")
    bool InitCoreSettings(bool bUseDefaults);

    UFUNCTION(BlueprintCallable, Category = "Core|Info")
    FString GetCoreVersion();

    UFUNCTION(BlueprintCallable, Category = "Core|Shutdown")
    bool ShutdownCore();

    UFUNCTION(BlueprintCallable, Category = "Core|Utilities")
    FString GenerateUniqueID(FString Prefix);

    UFUNCTION(BlueprintCallable, Category = "Core|Utilities")
    FString FormatString(FString Input, bool bToUpperCase);
};
