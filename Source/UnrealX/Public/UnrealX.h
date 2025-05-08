// © 2025 UnrealX, all rights reserved by ELife Studio

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealXModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
