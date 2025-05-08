#include "corekit/CoreFunctions.h"
#include "CoreMinimal.h"

bool UCoreFunctions::InitCoreSettings(bool bUseDefaults)
{
    if (bUseDefaults)
    {
        UE_LOG(LogTemp, Log, TEXT("SDK initialized with default settings."));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("SDK initialized with custom settings."));
    }
    return true;
}

FString UCoreFunctions::GetCoreVersion()
{
    FString SDKVersion = TEXT("1.0.0");
    return SDKVersion;
}

bool UCoreFunctions::ShutdownCore()
{
    UE_LOG(LogTemp, Log, TEXT("SDK shutdown successfully."));
    return true;
}

FString UCoreFunctions::GenerateUniqueID(FString Prefix)
{
    static int32 Counter = 0;
    FString UniqueID = FString::Printf(TEXT("%s_%d"), *Prefix, Counter++);
    return UniqueID;
}

FString UCoreFunctions::FormatString(FString Input, bool bToUpperCase)
{
    if (bToUpperCase)
    {
        Input = Input.ToUpper();
    }
    else
    {
        Input = Input.ToLower();
    }
    return Input;
}
