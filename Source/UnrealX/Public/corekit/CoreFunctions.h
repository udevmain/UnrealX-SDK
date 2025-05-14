// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "CoreFunctions.generated.h"

UCLASS(Blueprintable)
class UNREALX_API UCoreFunctions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Core|Settings")
    bool InitCoreSettings(bool bUseDefaults);

    UFUNCTION(BlueprintCallable, Category = "Core|Info")
    static FString GetCoreVersion();

    UFUNCTION(BlueprintCallable, Category = "Core|Shutdown")
    bool ShutdownCore();

    UFUNCTION(BlueprintCallable, Category = "Core|Utilities")
    static FString GenerateUniqueID(FString Prefix, int32 Length);

    UFUNCTION(BlueprintCallable, Category = "Core|Utilities")
    static FString FormatString(FString Input, bool bToUpperCase);
};
