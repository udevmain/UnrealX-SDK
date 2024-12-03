#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreDebugger.generated.h"

UCLASS()
class UnrealX_API UCoreDebugger : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Debug|Logs")
    static void LogCoreMessage(const FString& Message, bool bIsWarning);

    UFUNCTION(BlueprintCallable, Category = "Debug|Core")
    static void EnableCoreDebugMode(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Debug|Performance")
    static void LogPerformanceMetrics(const FString& MetricsType);

    UFUNCTION(BlueprintCallable, Category = "Debug|Errors")
    static void TrackError(int32 ErrorCode, const FString& ErrorMessage);
};
