#include "corekit/CoreErrorHandler.h"
#include "corekit/CoreUtils.h"

void UCoreErrorHandler::ReportError(int32 ErrorCode, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Error Code: %d, Message: %s"), ErrorCode, *ErrorMessage);
    UCoreUtils::SendMessageToServer("CoreErrorHandler", "ReportError(): null", FString::Printf(TEXT("Error Code: %d, Message: %s"), ErrorCode, *ErrorMessage));
}
