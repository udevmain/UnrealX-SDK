#include "corekit/CoreDebugger.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/LogMacros.h"

void UCoreDebugger::LogCoreMessage(const FString& Message, bool bIsWarning)
{
    if (bIsWarning)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    }
}

void UCoreDebugger::EnableCoreDebugMode(bool bEnable)
{
    if (bEnable)
    {
        UE_LOG(LogTemp, Log, TEXT("Debug Mode Enabled"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Debug Mode Disabled"));
    }
}

void UCoreDebugger::LogPerformanceMetrics(const FString& MetricsType)
{
    if (MetricsType.Equals(TEXT("FPS")))
    {
        UE_LOG(LogTemp, Log, TEXT("Logging FPS performance metrics"));
    }
    else if (MetricsType.Equals(TEXT("Memory")))
    {
        UE_LOG(LogTemp, Log, TEXT("Logging memory usage metrics"));
    }
}

void UCoreDebugger::TrackError(int32 ErrorCode, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Error Code: %d, Message: %s"), ErrorCode, *ErrorMessage);
}
