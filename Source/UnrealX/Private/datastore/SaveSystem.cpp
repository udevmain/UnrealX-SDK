#include "datastore/SaveSystem.h"
#include "Core/sdk_subsystem.h"
#include "UnrealX_Types.h"
#include "corekit/CoreUtils.h"
#include "JsonObjectConverter.h"
#include "Containers/UnrealString.h"

void USaveSystem::SavePlayerData(const FPlayerData& Data, FOnSaveComplete OnComplete)
{
    const FString Url = FString::Printf(TEXT("https://unrealx.space/api/save?PlayerID=%s"), *Data.PlayerID);
    
    FString AppID = Usdk_subsystem::GetAppID();

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    FJsonObjectConverter::UStructToJsonObject(FPlayerData::StaticStruct(), &Data, JsonObject.ToSharedRef(), 0, 0);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("appID", AppID);
    Request->SetContentAsString(OutputString);

    Request->OnProcessRequestComplete().BindStatic(&USaveSystem::OnSaveResponseReceived, OnComplete);
    Request->ProcessRequest();
}

void USaveSystem::OnSaveResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FOnSaveComplete OnComplete)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        OnComplete.ExecuteIfBound(false, TEXT("Request failed or invalid response."));
        UCoreUtils::SendMessageToServer("SaveSystem", "OnSaveResponeReceived: 34", "Request failed or invalid response.");
        return;
    }

    if (Response->GetResponseCode() != 200)
    {
        OnComplete.ExecuteIfBound(false, FString::Printf(TEXT("HTTP Error: %d"), Response->GetResponseCode()));
        UCoreUtils::SendMessageToServer("SaveSystem", "OnSaveResponeReceived: 41", FString::Printf(TEXT("HTTP Error: %d"), Response->GetResponseCode()));
        return;
    }

    OnComplete.ExecuteIfBound(true, TEXT(""));
}

void USaveSystem::LoadPlayerData(FString PlayerID, FOnLoadComplete Callback)
{
    FString Url = FString::Printf(TEXT("https://unrealx.space/api/load?PlayerID=%s"), *PlayerID);
    FString AppID = Usdk_subsystem::GetAppID();

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("appID", AppID);

    Request->OnProcessRequestComplete().BindStatic(&USaveSystem::OnLoadResponseReceived, Callback);
    Request->ProcessRequest();
}

void USaveSystem::OnLoadResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FOnLoadComplete Callback)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        Callback.ExecuteIfBound(false, TEXT("Ошибка запроса"), FPlayerData());
        return;
    }

    if (Response->GetResponseCode() != 200)
    {
        Callback.ExecuteIfBound(false, FString::Printf(TEXT("Ошибка сервера: %d"), Response->GetResponseCode()), FPlayerData());

        return;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        Callback.ExecuteIfBound(false, TEXT("JSON parsing error!"), FPlayerData());
        return;
    }

    FPlayerData PlayerData;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FPlayerData>(JsonObject.ToSharedRef(), &PlayerData))
    {
        Callback.ExecuteIfBound(false, TEXT("Ошибка десериализации JSON"), FPlayerData());
        return;
    }

    Callback.ExecuteIfBound(true, TEXT("Data uploaded successfully"), PlayerData);
}

bool USaveSystem::DeletePlayerData(FString PlayerID)
{
    FString Url = FString::Printf(TEXT("https://unrealx.space/api/delete?PlayerID=%s"), *PlayerID);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("DELETE");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
    
    return true;
}
