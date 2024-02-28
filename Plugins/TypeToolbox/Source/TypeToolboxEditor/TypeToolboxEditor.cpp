// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeToolboxEditor.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "UI/Graph/TypeToolboxPinFactory.h"
#include "UI/Property/DataTypeBaseCustomization.h"
#include "UI/Property/TableRowSelectorCustomization.h"
#include "UI/Property/DataTypeSelectorCustomization.h"
#include "TypeToolbox/DataType/DataTypeProperties.h"
#include "Tools/TypeToolboxTools.h"
#include "Utility/TypeToolboxUtils.h"

IMPLEMENT_MODULE(FTypeToolboxEditorModule, TypeToolboxEditor);

void FTypeToolboxEditorModule::StartupModule()
{
	TypeToolboxTools = MakeShared<FTypeToolboxTools>();
	
	if (!IsRunningCommandlet())
	{
		TypeToolboxTools->StartupTools();
	}
	
	// Register properties when the module is booted
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FTableRowSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTableRowSelectorCustomization::MakeInstance));
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(FDataTypeSelector::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTypeSelectorCustomization::MakeInstance));

	// Find all children of FDataTypeBaseId::StaticStruct (inclusive) and register them to use FDataTypeBaseCustomization...
	TArray<UScriptStruct*> DataTypeStructures = FTypeToolboxUtils::GetAllDataTypeBaseStruct();
	for (const UScriptStruct* Struct : DataTypeStructures)
	{
		PropertyEditorModule.RegisterCustomPropertyTypeLayout(Struct->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTypeBaseCustomization::MakeInstance));
	}
	
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	// Register custom pin factory.
	TypeToolboxPinFactory = MakeShareable(new FTypeToolboxPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(TypeToolboxPinFactory);
}

void FTypeToolboxEditorModule::ShutdownModule()
{
	if (!IsRunningCommandlet())
	{
		TypeToolboxTools->ShutdownTools();
	}
	
	// Unregister properties when the module is shutdown
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FTableRowSelector::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FDataTypeSelector::StaticStruct()->GetFName());

		// Find all children of FDataTypeBaseId::StaticStruct and unregister them from FDataTypeBaseCustomization...
		TArray<UScriptStruct*> FoundStructs = FTypeToolboxUtils::GetAllDataTypeBaseStruct();
		for (const UScriptStruct* Struct : FoundStructs)
		{
			PropertyModule.UnregisterCustomPropertyTypeLayout(Struct->GetFName());
		}
		
		PropertyModule.NotifyCustomizationModuleChanged();
	}

	// Unregister custom pin factory.
	FEdGraphUtilities::UnregisterVisualPinFactory(TypeToolboxPinFactory);
}