#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCoreInterface.generated.h"

UINTERFACE(Blueprintable)
class UnrealX_API UGameCoreInterface : public UInterface
{
    GENERATED_BODY()
};

class UnrealX_API IGameCoreInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Game|Events")
    virtual void OnGameStart() = 0;

    UFUNCTION(BlueprintCallable, Category = "Game|Events")
    virtual void OnGamePause() = 0;

    UFUNCTION(BlueprintCallable, Category = "Game|Events")
    virtual void OnGameResume() = 0;

    UFUNCTION(BlueprintCallable, Category = "Game|Events")
    virtual void OnGameExit() = 0;

    UFUNCTION(BlueprintCallable, Category = "Game|Save")
    virtual void OnGameSave() = 0;

    UFUNCTION(BlueprintCallable, Category = "Game|Save")
    virtual void OnGameLoad() = 0;
};
