// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FTypeToolboxTools;
class FTypeToolboxPinFactory;

class FTypeToolboxEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	
	/** Tools management class pointer. */
	TSharedPtr<FTypeToolboxTools> TypeToolboxTools;

	/** Type toolbox custom pin factory. */
	TSharedPtr<FTypeToolboxPinFactory> TypeToolboxPinFactory;
};
