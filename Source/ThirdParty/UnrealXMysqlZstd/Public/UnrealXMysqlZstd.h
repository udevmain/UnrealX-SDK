#pragma once

#include "Modules/ModuleManager.h"

class FUnrealXMysqlZstdModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
