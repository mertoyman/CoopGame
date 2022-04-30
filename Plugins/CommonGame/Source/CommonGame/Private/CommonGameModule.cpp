// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleManager.h"

/**
 * Implements the FCommonGameModule module.
 */
class FCommonGameModule : public IModuleInterface
{
public:
	FCommonGameModule();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

};

FCommonGameModule::FCommonGameModule()
{
}

void FCommonGameModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCommonGameModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
	
IMPLEMENT_MODULE(FCommonGameModule, CommonGame)