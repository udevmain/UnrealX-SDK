#include "corekit/CoreUtils.h"
#include "Misc/DateTime.h"
#include "Serialization/JsonSerializer.h"

FString UCoreUtils::GetCurrentDateTime()
{
    FDateTime CurrentDateTime = FDateTime::Now();
    return CurrentDateTime.ToString();
}

FString UCoreUtils::ConvertToJson(const UStruct* Data)
{
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(Data, JsonString);
    return JsonString;
}

void UCoreUtils::ParseJson(const FString& JsonString, UStruct* OutStruct)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, OutStruct, 0, 0);
    }
}
