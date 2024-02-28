// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeToolboxUtils.h"
#include "TypeToolbox/DataType/BaseDataType.h"

bool FTypeToolboxUtils::IsValidDataTypeBaseStruct(const UScriptStruct* Struct)
{
	const UScriptStruct* DataTypeBaseIdStruct = FDataTypeBase::StaticStruct();
	const bool bBasedOnDataTypeBase = DataTypeBaseIdStruct && Struct->IsChildOf(DataTypeBaseIdStruct) && (Struct != DataTypeBaseIdStruct);
	const bool bValidStruct = (Struct->GetOutermost() != GetTransientPackage());
	return bBasedOnDataTypeBase && bValidStruct;
}

TArray<UScriptStruct*> FTypeToolboxUtils::GetAllDataTypeBaseStruct()
{
	TArray<UScriptStruct*> FoundStructs;
	UScriptStruct* DataTypeBaseIdStruct = FDataTypeBase::StaticStruct();
	FoundStructs.Add(DataTypeBaseIdStruct);
	
	for (TObjectIterator<UScriptStruct> StructIt; StructIt; ++StructIt)
	{
		UScriptStruct* CurrentStruct = *StructIt;
		const bool bBasedOnDataTypeBase = DataTypeBaseIdStruct && CurrentStruct->IsChildOf(DataTypeBaseIdStruct) && (CurrentStruct != DataTypeBaseIdStruct);
		const bool bValidStruct = (CurrentStruct->GetOutermost() != GetTransientPackage());
		if (bBasedOnDataTypeBase && bValidStruct)
		{
			FoundStructs.Add(CurrentStruct);
		}
	}

	return FoundStructs;
}