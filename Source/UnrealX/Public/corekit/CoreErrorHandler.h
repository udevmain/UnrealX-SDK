#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreErrorHandler.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UnrealX_API UCoreErrorHandler : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Errors|Log")
    void ReportError(int32 ErrorCode, const FString& ErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "Errors|UI")
    void DisplayErrorPopup(const FString& ErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "Errors|Log")
    void ClearErrors();

private:
    UPROPERTY(BlueprintReadWrite, Category = "Errors|UI")
    TSubclassOf<UUserWidget*> ErrorWidgetClass;
};
