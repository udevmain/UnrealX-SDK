#include "datastore/SaveSystem.h"

void USaveSystem::SavePlayerData(int32 PlayerID, const FPlayerData& Data)
{
    FString Url = "https://unrealx.space/api/save";
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    FJsonObjectConverter::UStructToJsonObject(FPlayerData::StaticStruct(), &Data, JsonObject.ToSharedRef(), 0, 0);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(OutputString);
    Request->OnProcessRequestComplete().BindStatic(&USaveSystem::OnSaveResponseReceived);
    Request->ProcessRequest();
}

void USaveSystem::LoadPlayerData(int32 PlayerID)
{
    FString Url = FString::Printf(TEXT("https://unrealx.space/api/load?PlayerID=%d"), PlayerID);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->OnProcessRequestComplete().BindStatic(&USaveSystem::OnLoadResponseReceived);
    Request->ProcessRequest();
}

bool USaveSystem::DeletePlayerData(int32 PlayerID)
{
    FString Url = FString::Printf(TEXT("https://unrealx.space/api/delete?PlayerID=%d"), PlayerID);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("DELETE");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
    
    return true;
}

bool USaveSystem::RollbackPlayerData(int32 PlayerID, int32 VersionID)
{
    FString Url = FString::Printf(TEXT("https://unrealx.space/api/rollback?PlayerID=%d&VersionID=%d"), PlayerID, VersionID);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
    
    return true;
}

void USaveSystem::OnSaveResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Save Successful: %s"), *Response->GetContentAsString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Save Failed"));
    }
}

void USaveSystem::OnLoadResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FPlayerData LoadedData;
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            FJsonObjectConverter::JsonObjectToUStruct<FPlayerData>(JsonObject.ToSharedRef(), &LoadedData, 0, 0);
            UE_LOG(LogTemp, Log, TEXT("Load Successful: Player %d, Level %d"), LoadedData.PlayerID, LoadedData.Level);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Load Failed"));
    }
}