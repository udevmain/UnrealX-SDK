#include "datastore/SyncManager.h"

void USyncManager::SyncData(const FPlayerData& Data, bool bForceSync)
{
    FString Url = "https://unrealx.space/api/sync";
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
    Request->OnProcessRequestComplete().BindStatic(&USyncManager::OnSyncResponseReceived);
    Request->ProcessRequest();
}

bool USyncManager::HasPendingSync(int32 PlayerID)
{
    return PendingSyncs.Contains(PlayerID);
}

void USyncManager::ResolveConflicts(int32 PlayerID, EConflictResolution Strategy)
{
    FString Url = FString::Printf(TEXT("https://unrealx.space/api/resolve?PlayerID=%d&Strategy=%d"), PlayerID, (int32)Strategy);
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");
    Request->ProcessRequest();
}

void USyncManager::StartBackgroundSync()
{
    // Implement background sync logic here
    UE_LOG(LogTemp, Log, TEXT("Background sync started"));
}

void USyncManager::EnableDeltaSync(bool bEnable)
{
    bDeltaSyncEnabled = bEnable;
    UE_LOG(LogTemp, Log, TEXT("Delta sync %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void USyncManager::RegisterSyncEventHandler(FSyncEventHandler Handler)
{
    // It will be necessary to implement the event handler registration logic later.
}

void USyncManager::OnSyncResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Sync Successful: %s"), *Response->GetContentAsString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Sync Failed"));
    }
}

TArray<int32> USyncManager::PendingSyncs;
bool USyncManager::bDeltaSyncEnabled = false;