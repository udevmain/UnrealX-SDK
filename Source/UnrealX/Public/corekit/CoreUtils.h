#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreUtils.generated.h"

UCLASS()
class UnrealX_API UCoreUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Utils|DateTime")
    static FString GetCurrentDateTime();

    UFUNCTION(BlueprintCallable, Category = "Utils|JSON")
    static FString ConvertToJson(const UStruct* Data);

    UFUNCTION(BlueprintCallable, Category = "Utils|JSON")
    static void ParseJson(const FString& JsonString, UStruct* OutStruct);

    UFUNCTION(BlueprintCallable, Category = "Utils|Report")
    static void SendMessageToServer(const FString& Username, const FString& Email, const FString& Message)
};
