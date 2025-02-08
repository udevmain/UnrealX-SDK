#include "corekit/CoreUtils.h"
#include "Misc/DateTime.h"
#include "Serialization/JsonSerializer.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Core/sdk_subsystem.h"

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

void UCoreUtils::SendMessageToServer(const FString& Username, const FString& Email, const FString& Message)
{
    if (!FHttpModule::Get().IsHttpEnabled())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP не включен в проекте!"));
        return;
    }

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    FString Url = TEXT("https://unrealx.space/api/addMessage");
    Request->SetURL(Url);
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    JsonObject->SetStringField("username", Username);
    JsonObject->SetStringField("email", Email);
    JsonObject->SetStringField("message", Message);
    JsonObject->SetStringField("appID", Usdk_subsystem::GetAppID());

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        if (bWasSuccessful && Response.IsValid())
        {
            UE_LOG(LogTemp, Log, TEXT("Сообщение успешно отправлено: %s"), *Response->GetContentAsString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Ошибка при отправке сообщения!"));
        }
    });

    Request->ProcessRequest();
}
