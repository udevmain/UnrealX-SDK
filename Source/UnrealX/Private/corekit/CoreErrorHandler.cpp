#include "corekit/CoreErrorHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UCoreErrorHandler::ReportError(int32 ErrorCode, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Error Code: %d, Message: %s"), ErrorCode, *ErrorMessage);
}

void UCoreErrorHandler::DisplayErrorPopup(const FString& ErrorMessage)
{
    if (UUserWidget* ErrorWidget = CreateWidget<UUserWidget>(GetWorld(), ErrorWidgetClass))
    {
        ErrorWidget->SetVisibility(ESlateVisibility::Visible);
        ErrorWidget->AddToViewport();
    }
}

void UCoreErrorHandler::ClearErrors()
{
    UE_LOG(LogTemp, Log, TEXT("All errors cleared"));
}
