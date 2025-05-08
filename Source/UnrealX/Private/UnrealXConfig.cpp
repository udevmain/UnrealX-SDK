#include "UnrealXConfig.h"
#include "Misc/ConfigCacheIni.h"
#include "CoreGlobals.h"

FConfigReturn UUnrealXConfig::GetConfigVar(const FString& Key)
{
	FConfigReturn CR;
	if (!GConfig)
	{
		UE_LOG(LogTemp, Error, TEXT("[GConfig Error]"));
		CR.bIsSuccess = false;
		CR.Result = "";
		return CR;
	}

	FString value;
	const FString Section = "/Script/UnrealX.Config";
	const FString Filename = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + TEXT("/UnrealX.ini"));
	GConfig->Flush(true, Filename);
	if (GConfig->GetString(GetData(Section), GetData(Key), value, GetData(Filename)))
	{
		CR.bIsSuccess = true;
		CR.Result = value;
		return CR;
	}

	UE_LOG(LogTemp, Error, TEXT("[Error reading config file, Please ensure UnrealX.ini is setup correctly]"));
	CR.bIsSuccess = false;
	CR.Result = "";
	return CR;
}
