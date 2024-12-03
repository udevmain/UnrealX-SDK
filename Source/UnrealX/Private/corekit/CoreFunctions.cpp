#include "corekit/CoreFunctions.h"
#include "CoreMinimal.h"

bool CoreFunctions::InitCoreSettings(bool bUseDefaults)
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

FString CoreFunctions::GetCoreVersion()
{
    FString SDKVersion = TEXT("1.0.0");
    return SDKVersion;
}

bool CoreFunctions::ShutdownCore()
{
    UE_LOG(LogTemp, Log, TEXT("SDK shutdown successfully."));
    return true;
}

FString CoreFunctions::GenerateUniqueID(FString Prefix)
{
    static int32 Counter = 0;
    FString UniqueID = FString::Printf(TEXT("%s_%d"), *Prefix, Counter++);
    return UniqueID;
}

FString CoreFunctions::FormatString(FString Input, bool bToUpperCase)
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
