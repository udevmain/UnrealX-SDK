#include "corekit/CoreSaveSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"

bool UCoreSaveSystem::SaveGameData(const FString& SaveSlotName, const FGameSave& Data)
{
    FString SaveDataPath = FPaths::ProjectSavedDir() + SaveSlotName + TEXT(".json");
    FString JsonString;
    FJsonObjectConverter::UStructToJsonObjectString(Data, JsonString);

    return FFileHelper::SaveStringToFile(JsonString, *SaveDataPath);
}

FGameSave UCoreSaveSystem::LoadGameData(const FString& SaveSlotName)
{
    FString SaveDataPath = FPaths::ProjectSavedDir() + SaveSlotName + TEXT(".json");
    FString JsonString;
    FGameSave LoadedData;

    if (FFileHelper::LoadFileToString(JsonString, *SaveDataPath))
    {
        FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoadedData, 0, 0);
    }

    return LoadedData;
}

void UCoreSaveSystem::AutoSave()
{
    // soon
    UE_LOG(LogTemp, Log, TEXT("Auto-save in progress..."));
}

bool UCoreSaveSystem::DeleteSaveSlot(const FString& SaveSlotName)
{
    FString SaveDataPath = FPaths::ProjectSavedDir() + SaveSlotName + TEXT(".json");
    return FPaths::FileExists(SaveDataPath) ? IFileManager::Get().Delete(*SaveDataPath) : false;
}
