// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreErrorHandler.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALX_API UCoreErrorHandler : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Errors|Log")
    void ReportError(int32 ErrorCode, const FString& ErrorMessage);
};
