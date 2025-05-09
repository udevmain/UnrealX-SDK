#include "corekit/CoreUtils.h"
#include "Misc/DateTime.h"
#include "Serialization/JsonSerializer.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilitiesClasses.h"
#include "Core/sdk_subsystem.h"
#include <JsonObjectConverter.h>
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonValue.h"

FString UCoreUtils::GetCurrentDateTime()
{
    FDateTime CurrentDateTime = FDateTime::Now();
    return CurrentDateTime.ToString();
}

FString UCoreUtils::ConvertToJson(const UStruct* Data)
{
    FString JsonString;
    // FJsonObjectConverter::UStructToJsonObjectString(Data, JsonString);
    return JsonString;
}

void UCoreUtils::ParseJson(const FString& JsonString, UStruct* OutStruct)
{ 
    /*
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, OutStruct, 0, 0);
    }
    */
    //maybe...
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

FString UCoreUtils::GetOnlinePlatformName()
{
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (!OnlineSub)
    {
        return TEXT("Unknown");
    }

    const FString SubsystemName = OnlineSub->GetSubsystemName().ToString().ToLower();

    if (SubsystemName.Contains(TEXT("steam")))
    {
        return TEXT("Steam");
    }
    else if (SubsystemName.Contains(TEXT("psn")) || SubsystemName.Contains(TEXT("ps5")))
    {
        return TEXT("PlayStation");
    }
    else if (SubsystemName.Contains(TEXT("xbox")) || SubsystemName.Contains(TEXT("live")))
    {
        return TEXT("Xbox");
    }
    else if (SubsystemName.Contains(TEXT("epic")))
    {
        return TEXT("Epic");
    }
    else if (SubsystemName.Contains(TEXT("ios")))
    {
        return TEXT("iOS");
    }
    else if (SubsystemName.Contains(TEXT("android")))
    {
        return TEXT("Android");
    }
    else if (SubsystemName.Contains(TEXT("null")))
    {
        return TEXT("Offline");
    }

    return SubsystemName;
}

FString UCoreUtils::GetUserPlatformID()
{
    // Get the current online subsystem (Steam, EOS, etc.)
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!OnlineSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found."));
        return TEXT("Unknown");
    }

    // Get identity interface
    IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
    if (!IdentityInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("IdentityInterface is invalid."));
        return TEXT("Unknown");
    }

    // Get the local user ID (usually 0 for single-player or first logged-in user)
    TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);
    if (!UserId.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get UniquePlayerId."));
        return TEXT("Unknown");
    }

    // Convert the ID to string (platform-specific format)
    return UserId->ToString();
}

bool UCoreUtils::MakeJsonStringFromMap(const TMap<FString, FString>& InMap, FString& OutJsonString)
{
    TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    for (const TPair<FString, FString>& Pair : InMap)
    {
        JsonObject->SetStringField(Pair.Key, Pair.Value);
    }

    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
    return FJsonSerializer::Serialize(JsonObject, Writer);
}

bool UCoreUtils::MakeAdvancedJsonString(const TArray<FJsonKeyValue>& Pairs, FString& OutJsonString)
{
    TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    for (const FJsonKeyValue& Pair : Pairs)
    {
        switch (Pair.Type)
        {
        case EJsonValueType::String:
            JsonObject->SetStringField(Pair.Key, Pair.Value);
            break;

        case EJsonValueType::Number:
            JsonObject->SetNumberField(Pair.Key, FCString::Atod(*Pair.Value));
            break;

        case EJsonValueType::Boolean:
            JsonObject->SetBoolField(Pair.Key, Pair.Value.ToBool());
            break;

        case EJsonValueType::JsonObject:
        {
            TSharedPtr<FJsonObject> NestedJson;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Pair.Value);
            if (FJsonSerializer::Deserialize(Reader, NestedJson) && NestedJson.IsValid())
            {
                JsonObject->SetObjectField(Pair.Key, NestedJson);
            }
            break;
        }
        }
    }

    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonString);
    return FJsonSerializer::Serialize(JsonObject, Writer);
}
