// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeToolboxGraph.h"

// NOTE: Defined this module to prevent having to put the K2Nodes in the TypeToolbox module as that is game / runtime
// only code and I'd like to keep it that way. Was getting errors about the K2Nodes needing to exist in a UncookedOnly
// or Development which was not valid for the editor module...

IMPLEMENT_MODULE(FTypeToolboxGraphModule, TypeToolboxGraph);

void FTypeToolboxGraphModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FTypeToolboxGraphModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

