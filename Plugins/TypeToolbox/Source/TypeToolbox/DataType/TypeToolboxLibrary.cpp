// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeToolboxLibrary.h"
#include "DataTypeSubsystem.h"
#include "Kismet/DataTableFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTypeToolboxLibrary, Log, All);

bool UTypeToolboxLibrary::IsDataTypeValid(const FDataTypeBase InDataType)
{
	UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	return DataTypeSubsystem->IsValidDataType(InDataType);
}

TOptional<FDataTypeSettings> UTypeToolboxLibrary::GetDataTypeSettingsFromIndex(const int32 InIndex)
{
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TOptional<FDataTypeSettings>();
	}

	const TMap<FDataTypeSelector, FDataTypeSettings> DataTypesSettingsMap = DataTypeSubsystem->GetDataTypesSettings();
	TArray<FDataTypeSettings> DataTypesSettings;
	DataTypesSettingsMap.GenerateValueArray(DataTypesSettings);
	if (DataTypesSettings.Num() <= 0 || !DataTypesSettings.IsValidIndex(InIndex))
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Cannot find the data type settings at index %s, it is out of bounds and doesnt exist!"), ANSI_TO_TCHAR(__FUNCTION__), *FString::FromInt(InIndex));
		return TOptional<FDataTypeSettings>();
	}

	return DataTypesSettings[InIndex];
}

TOptional<FDataTypeSettings> UTypeToolboxLibrary::GetDataTypeSettingsFromTypeStruct(const UScriptStruct* InScriptStruct)
{
	if (!InScriptStruct)
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find settings for null struct!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TOptional<FDataTypeSettings>();
	}
	
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TOptional<FDataTypeSettings>();
	}

	const TMap<FDataTypeSelector, FDataTypeSettings> DataTypesSettings = DataTypeSubsystem->GetDataTypesSettings();
	const FDataTypeSelector FindDataType = FDataTypeSelector(InScriptStruct);
	if (!DataTypesSettings.Contains(FindDataType))
	{
		return TOptional<FDataTypeSettings>();
	}

	return DataTypesSettings.FindRef(FindDataType);
}

bool UTypeToolboxLibrary::GetDataTypeSettingsFromType(FDataTypeSelector InType, FDataTypeSettings& OutFoundSettings)
{
	const TOptional<FDataTypeSettings> FoundSettings = GetDataTypeSettingsFromTypeStruct(InType.SelectedStructType);
	if (!FoundSettings.IsSet())
	{
		return false;
	}

	OutFoundSettings = FoundSettings.GetValue();
	return true;
}

UDataTable* UTypeToolboxLibrary::GetDataTableFromTypeRowName(const FString& InRowName)
{
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	const TMap<FString, UDataTable*> RowToTableMap = DataTypeSubsystem->GetRowNameToTableMap();
	if (!RowToTableMap.Contains(InRowName))
	{
		return nullptr;
	}

	return RowToTableMap.FindRef(InRowName);
}

UDataTable* UTypeToolboxLibrary::GetDataTableFromTypeStruct(const UScriptStruct* InScriptStruct)
{
	if (!InScriptStruct)
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find table for null struct type!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	// If its the FDataTypeBase value type, we want to return a composite table including all values...
	if (InScriptStruct == FDataTypeBase::StaticStruct())
	{
		UDataTable* FoundTable = nullptr;
		GetAllDataTypesAsDataTable(FoundTable);
		return FoundTable;
	}
	
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}
	
	const TMap<UScriptStruct*, UDataTable*> StructToTableMap = DataTypeSubsystem->GetStructToTableMap();
	if (!StructToTableMap.Contains(InScriptStruct))
	{
		return nullptr;
	}

	return StructToTableMap.FindRef(InScriptStruct);
}

bool UTypeToolboxLibrary::GetDataTableFromType(FDataTypeSelector InType, UDataTable*& OutDataTable)
{
	UDataTable* FoundTable = GetDataTableFromTypeStruct(InType.SelectedStructType);
	if (!FoundTable)
	{
		return false;
	}

	OutDataTable = FoundTable;
	return true;
}

bool UTypeToolboxLibrary::GetDataTableFromValue(const FDataTypeBase InTypeValue, UDataTable*& OutDataTable)
{
	if (!InTypeValue.IsValid())
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find table for invalid type value: %s!"), ANSI_TO_TCHAR(__FUNCTION__), *InTypeValue.RowNameAsString());
		return false;
	}
	
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}
	
	TArray<UDataTable*> LoadedTables = DataTypeSubsystem->GetLoadedDataTables();
	if (!LoadedTables.IsValidIndex(InTypeValue.DataTableIndex))
	{
		return false;
	}

	OutDataTable = LoadedTables[InTypeValue.DataTableIndex];
	return true;
}

UScriptStruct* UTypeToolboxLibrary::GetDataTypeRowType(const FDataTypeBase InTypeValue)
{
	const TOptional<FDataTypeSettings> FoundSettings = GetDataTypeSettingsFromIndex(InTypeValue.DataTableIndex);
	if (!FoundSettings.IsSet())
	{
		return nullptr;
	}

	return FoundSettings->DataTableRowType.SelectedStructType;
}

bool UTypeToolboxLibrary::K2_GetDataTableRowFromValue(const FDataTypeBase InTypeValue, FTableRowBase& OutRow)
{
	UDataTable* FoundDataTable = nullptr;
	if (!GetDataTableFromValue(InTypeValue, FoundDataTable))
	{
		return false;
	}

	return UDataTableFunctionLibrary::Generic_GetDataTableRowFromName(FoundDataTable, InTypeValue.RowName, &OutRow);
}

TSet<FDataTypeBase> UTypeToolboxLibrary::GetAllDataTypeValues()
{
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TSet<FDataTypeBase>();
	}

	return DataTypeSubsystem->GetAllDataTypeValues();
}

TSet<FDataTypeBase> UTypeToolboxLibrary::GetAllDataTypeValuesForStruct(const UScriptStruct* InScriptStruct)
{
	if (!InScriptStruct)
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find data type values for null struct type!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TSet<FDataTypeBase>();
	}

	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TSet<FDataTypeBase>();
	}
	
	const UDataTable* FoundDataTable = GetDataTableFromTypeStruct(InScriptStruct);
	if (!FoundDataTable || FoundDataTable->GetRowMap().Num() <= 0)
	{
		return TSet<FDataTypeBase>();
	}

	const int32 DataTableIndex = DataTypeSubsystem->GetDataTableIndex(FoundDataTable);
	if (DataTableIndex < 0)
	{
		return TSet<FDataTypeBase>();
	}
	
	TSet<FDataTypeBase> ReturnSet = TSet<FDataTypeBase>();
	for (const TPair<FName, uint8*>& RowPair : FoundDataTable->GetRowMap())
	{
		const FName& RowName = RowPair.Key;
		if (RowName.IsEqual(NAME_None))
		{
			continue;
		}
		
		FDataTypeBase NewDataType = FDataTypeBase(RowName, DataTableIndex);
		ReturnSet.Add(NewDataType);
	}

	return ReturnSet;
}

TSet<FDataTypeBase> UTypeToolboxLibrary::GetAllDataTypeValuesForType(FDataTypeSelector InType)
{
	if (!InType.SelectedStructType)
	{
		return TSet<FDataTypeBase>();
	}
	
	return GetAllDataTypeValuesForStruct(InType.SelectedStructType);
}

int32 UTypeToolboxLibrary::GetStatusMaskForValue(const FDataTypeBase InTypeValue)
{
	// Can assume that all types use the required FTypeTableRowBase as their data type row structure.
	const FTypeTableRowBase* FoundDataTypeRow = GetDataTableRowFromValue<FTypeTableRowBase>(InTypeValue);
	if (!FoundDataTypeRow)
	{
		return 0;
	}
	
	return FoundDataTypeRow->StatusMask;
}

FTypeUserInterfaceInfo UTypeToolboxLibrary::GetUserInterfaceInfoFromValue(const FDataTypeBase InTypeValue)
{
	// Can assume that all types use the required FTypeTableRowBase as their data type row structure.
	const FTypeTableRowBase* FoundDataTypeRow = GetDataTableRowFromValue<FTypeTableRowBase>(InTypeValue);
	if (!FoundDataTypeRow)
	{
		return FTypeUserInterfaceInfo();
	}

	return FoundDataTypeRow->UserInterfaceInfo;
}

bool UTypeToolboxLibrary::GetAllDataTypesAsDataTable(UDataTable*& OutDataTable)
{
	OutDataTable = nullptr;
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	const TArray<UDataTable*> LoadedDataTables = DataTypeSubsystem->GetLoadedDataTables();
	if (LoadedDataTables.Num() <= 0)
	{
		return false;
	}

	const FName UniqueTableName = MakeUniqueObjectName(GetTransientPackage(), UDataTable::StaticClass(), FName("DT_AllDataTypes_"));
	UDataTable* AllDataTablesTable = NewObject<UDataTable>(GetTransientPackage(), UniqueTableName);
	if (!AllDataTablesTable)
	{
		return false;
	}

	for (UDataTable* Table : LoadedDataTables)
	{
		if (!Table)
		{
			continue;
		}

		const TMap<FName, uint8*> TableRowMap = Table->GetRowMap();
		for (const TPair<FName, uint8*>& TableRowPair : TableRowMap)
		{
			FTableRowBase RowAsBase = *reinterpret_cast<FTableRowBase*>(TableRowPair.Value);
			AllDataTablesTable->AddRow(TableRowPair.Key, RowAsBase);
		}
	}

	OutDataTable = AllDataTablesTable;
	return true;
}

int32 UTypeToolboxLibrary::GetDataTableIndex(const UDataTable* InDataTable)
{
	if (!InDataTable)
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find table index for invalid/null data table!"), ANSI_TO_TCHAR(__FUNCTION__));
		return -1;
	}
	
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return -1;
	}

	return DataTypeSubsystem->GetDataTableIndex(InDataTable);
}

bool UTypeToolboxLibrary::EqualEqual_DataTypeDataType(const FDataTypeBase A, const FDataTypeBase B)
{
	return A == B;
}

bool UTypeToolboxLibrary::NotEqual_DataTypeDataType(const FDataTypeBase A, const FDataTypeBase B)
{
	return A != B;
}

template <typename RowType_T>
RowType_T* UTypeToolboxLibrary::GetDataTableRowFromValue(const FDataTypeBase InTypeValue)
{
	if (!InTypeValue.IsValid())
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find table row for invalid type value: %s!"), ANSI_TO_TCHAR(__FUNCTION__), *InTypeValue.RowNameAsString());
		return nullptr;
	}
	
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogTypeToolboxLibrary, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}
	
	TArray<UDataTable*> LoadedTables = DataTypeSubsystem->GetLoadedDataTables();
	if (!LoadedTables.IsValidIndex(InTypeValue.DataTableIndex))
	{
		return nullptr;
	}
	
	return LoadedTables[InTypeValue.DataTableIndex]->FindRow<RowType_T>(InTypeValue.RowName, "GetDataTableRowFromValue", false);
}

template <typename RowType_T>
TArray<RowType_T*> UTypeToolboxLibrary::GetDataTableRowsFromTypeStruct(const UScriptStruct* InScriptStruct)
{
	if (!InScriptStruct)
	{
		UE_LOG(LogTypeToolboxLibrary, Warning, TEXT("%s: Attempting to find table rows for null struct type!"), ANSI_TO_TCHAR(__FUNCTION__));
		return TArray<RowType_T*>();
	}

	const UDataTable* FoundTable = GetDataTableFromTypeStruct(InScriptStruct);
	if (!FoundTable)
	{
		return TArray<RowType_T*>();
	}

	TArray<RowType_T*> FoundRows;
	FoundTable->GetAllRows<RowType_T>("GetDataTableRowsFromTypeStruct", FoundRows);
	return FoundRows;
}