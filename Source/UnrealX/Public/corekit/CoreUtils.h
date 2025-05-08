// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreUtils.generated.h"

UCLASS()
class UNREALX_API UCoreUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Utils|DateTime")
    static FString GetCurrentDateTime();

    UFUNCTION(BlueprintCallable, Category = "Utils|JSON")
    static FString ConvertToJson(const UStruct* Data);

    UFUNCTION(BlueprintCallable, Category = "Utils|JSON")
    static void ParseJson(const FString& JsonString, UStruct* OutStruct);

    /*
        Sends a message to the server.
        Note:
        --If an error message is sent to the server inside the function,
        --the function and line number will be passed as "Username"
        --and "Email", and the error itself will be sent to "message".
    */
    UFUNCTION(BlueprintCallable, Category = "Utils|Report")
    static void SendMessageToServer(const FString& Username, const FString& Email, const FString& Message);

    /** Returns the user's online platform (e.g. Steam, Epic Games, PSN, etc) */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utils|Online")
    static FString GetOnlinePlatformName();

    /** Returns the user's unique platform ID as string (e.g. SteamID64, Epic Account ID, etc.) */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utils|Online")
    static FString GetUserPlatformID();
};
