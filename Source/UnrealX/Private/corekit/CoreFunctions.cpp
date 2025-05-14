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

FString UCoreFunctions::GenerateUniqueID(FString Prefix, int32 Length)
{
    const FString Characters = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    FString RandomID;
    for (int32 i = 0; i < Length; ++i)
    {
        int32 Index = FMath::RandRange(0, Characters.Len() - 1);
        RandomID.AppendChar(Characters[Index]);
    }
    FString OutUniqueID = Prefix + RandomID;
    return OutUniqueID;
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
