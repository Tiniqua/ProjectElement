// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeToolboxPinFactory.h"
#include "Pin/SDataTypeSelectorPin.h"
#include "Pin/SListDataTypePin.h"
#include "Pin/SListTableRowPin.h"
#include "TypeToolbox/DataType/DataTypeProperties.h"
#include "TypeToolboxEditor/Utility/TypeToolboxUtils.h"

TSharedPtr<SGraphPin> FTypeToolboxPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	
	if (InPin->PinType.PinCategory == K2Schema->PC_Struct && InPin->PinType.PinSubCategoryObject == FDataTypeSelector::StaticStruct())
	{
		return SNew(SListDataTypePin, InPin);
	}

	if (InPin->PinType.PinCategory == K2Schema->PC_Struct && InPin->PinType.PinSubCategoryObject == FTableRowSelector::StaticStruct())
	{
		return SNew(SListTableRowPin, InPin);
	}

	TArray<UScriptStruct*> DataTypeStructs = FTypeToolboxUtils::GetAllDataTypeBaseStruct();
	for (UScriptStruct* Struct : DataTypeStructs)
	{
		if (InPin->PinType.PinCategory == K2Schema->PC_Struct && InPin->PinType.PinSubCategoryObject == Struct)
		{
			return SNew(SDataTypeSelectorPin, InPin)
			.SelectedStructType(Struct);
		}
	}

	return nullptr;
}
