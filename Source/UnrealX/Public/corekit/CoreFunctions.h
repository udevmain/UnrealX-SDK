#pragma once

#include "CoreMinimal.h"
#include "CoreFunctions.generated.h"

UCLASS(Blueprintable)
class UnrealX_API UCoreFunctions : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Core|Settings")
    static bool InitCoreSettings(bool bUseDefaults);

    UFUNCTION(BlueprintCallable, Category = "Core|Info")
    static FString GetCoreVersion();

    UFUNCTION(BlueprintCallable, Category = "Core|Shutdown")
    static bool ShutdownCore();

    UFUNCTION(BlueprintCallable, Category = "Core|Utilities")
    static FString GenerateUniqueID(FString Prefix);

    UFUNCTION(BlueprintCallable, Category = "Core|Utilities")
    static FString FormatString(FString Input, bool bToUpperCase);
};
