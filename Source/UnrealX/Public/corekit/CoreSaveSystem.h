#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreSaveSystem.generated.h"

USTRUCT(BlueprintType)
struct UnrealX_API FGameSave
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString SaveData;

    UPROPERTY(BlueprintReadWrite)
    int32 PlayerLevel;
};

UCLASS()
class UnrealX_API UCoreSaveSystem : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Save|Game")
    static bool SaveGameData(const FString& SaveSlotName, const FGameSave& Data);

    UFUNCTION(BlueprintCallable, Category = "Save|Game")
    static FGameSave LoadGameData(const FString& SaveSlotName);

    UFUNCTION(BlueprintCallable, Category = "Save|Game")
    static void AutoSave();

    UFUNCTION(BlueprintCallable, Category = "Save|Game")
    static bool DeleteSaveSlot(const FString& SaveSlotName);
};
