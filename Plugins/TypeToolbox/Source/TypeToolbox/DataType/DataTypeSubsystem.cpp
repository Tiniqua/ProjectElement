// Fill out your copyright notice in the Description page of Project Settings.

#include "DataTypeSubsystem.h"
#include "TypeToolboxSettings.h"
#include "Engine/DataTable.h"

#if WITH_EDITOR
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#endif

void UDataTypeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Setup();
}

void UDataTypeSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Cleanup();
}

void UDataTypeSubsystem::MarkSubsystemDirty()
{
	Cleanup();
	Setup();

#if WITH_EDITOR
	Editor_Refreshed();
#endif
}

void UDataTypeSubsystem::Cleanup()
{
#if WITH_EDITOR

	for (UDataTable* LoadedTable : LoadedDataTables)
	{
		if (!LoadedTable)
		{
			continue;
		}
			
		LoadedTable->OnDataTableChanged().RemoveAll(this);
	}
	
	bNeedsRefresh = false;
	DataTypeLoadState.Empty();
#endif

	bInitialised = false;
	DataTypeSettings.Empty();
	LoadedDataTables.Empty();
	RowNameToTableMap.Empty();
	StructToTableMap.Empty();
	AllDataTypes.Empty();
}

void UDataTypeSubsystem::Setup()
{
	// Cache current settings.
	const UTypeToolboxSettings* TypeToolboxSettings = GetDefault<UTypeToolboxSettings>();
	if (!TypeToolboxSettings)
	{
		UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: Cannot access UTypeToolboxSettings CDO?! Failed to setup UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	DataTypeSettings = TypeToolboxSettings->DataTypes;
	if (DataTypeSettings.Num() <= 0)
	{
		UE_LOG(LogDataTypeSubsystem, Log, TEXT("%s: No need to setup data type subsystem as the DataType settings are empty."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	// Set initialised, this will be set to false if any errors are found while initialising.
	bInitialised = true;
	
	// Load data tables from DataTypeSettings for each defined data type.
	uint32 DataTypeIndex = 0;
	for (const TPair<FDataTypeSelector, FDataTypeSettings> DataTypePair : DataTypeSettings)
	{
		const FDataTypeSelector& SelectedStructure = DataTypePair.Key;
		FDataTypeSettings TypeSettings = DataTypePair.Value;

		// Check there is a valid key structure set for the type.
		UScriptStruct* StructureType = SelectedStructure.SelectedStructType;
		if (!StructureType)
		{
			UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: Data type defined in type toolbox settings at index: %s is empty!."), ANSI_TO_TCHAR(__FUNCTION__), *FString::FromInt(DataTypeIndex));
			bInitialised = false;
			continue;
		}

		// Check and make sure the struct data type has not been used more than once.
		if (StructToTableMap.Contains(StructureType))
		{
			UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: Datatype at index: %s has re-used the data type definition %s. This should not exist more than once!?"),
				ANSI_TO_TCHAR(__FUNCTION__), *FString::FromInt(DataTypeIndex), *StructureType->GetName());
			Editor_UpdateTypeLoadState(false, StructureType);
			continue;
		}

		// Check and load the data table and validate the type settings.
		FString LoadTableMsg;
		UDataTable* LoadedTable = nullptr;
		if (!TypeSettings.ValidateAndLoadDataTable(LoadedTable, LoadTableMsg))
		{
			UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: Type setting validation for %s failed: %s"), ANSI_TO_TCHAR(__FUNCTION__), *StructureType->GetName(), *LoadTableMsg);
			Editor_UpdateTypeLoadState(false, StructureType);
			continue;
		}
		
		// Loop through each row and validate/cache data type row entry.
		int32 RowIndex = -1;
		for (const TPair<FName, uint8*>& RowPair : LoadedTable->GetRowMap())
		{
			RowIndex++;
			const FName& RowName = RowPair.Key;
			const FString& RowNameString = RowName.ToString();
			
			// Check the row name starts with the defined TypeRowPrefix.
			if (!RowNameString.StartsWith(TypeSettings.TypeRowPrefix))
			{
				UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: Row name %s at row index %s in DataTable: %s does not start with valid type row prefix: %s. This data type definition will be skipped and this type will be marked as failed to load succesfully.."),
				ANSI_TO_TCHAR(__FUNCTION__), *RowName.ToString(), *FString::FromInt(RowIndex), *LoadedTable->GetName(), *TypeSettings.TypeRowPrefix);
				Editor_UpdateTypeLoadState(false, StructureType);
				continue;
			}

			// Check for duplicate row names that exist within the system.
			if (RowNameToTableMap.Contains(RowName.ToString()))
			{
				const UDataTable* DuplicateLocationTable = RowNameToTableMap.FindRef(RowName.ToString());
				UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: Row name %s on row index: %s is a duplicate in DataTable: %s. Same row name found in loaded datatable %s for type %s!"),
				ANSI_TO_TCHAR(__FUNCTION__), *RowName.ToString(), *FString::FromInt(RowIndex), *LoadedTable->GetName(), (DuplicateLocationTable ? *DuplicateLocationTable->GetName() : *FString("Null table")), *StructureType->GetName());
				Editor_UpdateTypeLoadState(false, StructureType);
				continue;
			}

			// Store all rows for quick data table lookup from row names.
			RowNameToTableMap.Add(RowName.ToString(), LoadedTable);
			
			// Store all possible data type's.
			FDataTypeBase BaseTypeId = FDataTypeBase(RowName, DataTypeIndex);
			AllDataTypes.Add(BaseTypeId);
		}
		
		// If validation succeeded store data table and type name mapped to that data table.
		LoadedDataTables.Add(LoadedTable);
		StructToTableMap.Add(StructureType, LoadedTable);
		DataTypeIndex++;

		// Flag load success for this type name if not previously failed.
		UE_LOG(LogDataTypeSubsystem, Log, TEXT("%s: Loaded database table %s for data type %s succesfully."), ANSI_TO_TCHAR(__FUNCTION__), *LoadedTable->GetName(), *StructureType->GetName());
		Editor_UpdateTypeLoadState(true, StructureType);
	}

	if (bInitialised)
	{
#if WITH_EDITOR

		// Foreach data table listen for changes.
		for (UDataTable* LoadedTable : LoadedDataTables)
		{
			if (!LoadedTable)
			{
				continue;
			}
			
			LoadedTable->OnDataTableChanged().AddUFunction(this, "Editor_DataTableModified");
		}

		// Mark success editor popup.
		const FText NotificationText = FText::FromString("Data Type Validation Succeeded!");
		FNotificationInfo FailedValidationMsg = FNotificationInfo(NotificationText);
		FailedValidationMsg.bFireAndForget = true;
		FailedValidationMsg.bUseSuccessFailIcons = true;
		FailedValidationMsg.ExpireDuration = 3.0f;
		TSharedPtr<SNotificationItem> SuccessNotify = FSlateNotificationManager::Get().AddNotification(FailedValidationMsg);
		if (SuccessNotify.IsValid())
		{
			SuccessNotify->SetCompletionState(SNotificationItem::CS_Success);
		}
#endif	
		
		UE_LOG(LogDataTypeSubsystem, Log, TEXT("%s: Loaded data type subsystem succesfully."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

#if WITH_EDITOR

	// Mark failure editor popup.
	const FText NotificationText = FText::FromString("Data Type Validation Failed!");
	FNotificationInfo FailedValidationMsg = FNotificationInfo(NotificationText);
	FailedValidationMsg.SubText = FText::FromString("Reloading the data types subsystem failed, the validation has found some errors. Please check your engine output log!");
	FailedValidationMsg.bFireAndForget = true;
	FailedValidationMsg.bUseThrobber = true;
	FailedValidationMsg.ExpireDuration = 10.0f;
	FailedValidationMsg.bUseSuccessFailIcons = true;
	TSharedPtr<SNotificationItem> FailNotify = FSlateNotificationManager::Get().AddNotification(FailedValidationMsg);
	if (FailNotify.IsValid())
	{
		FailNotify->SetCompletionState(SNotificationItem::CS_Fail);
	}
#endif

	UE_LOG(LogDataTypeSubsystem, Error, TEXT("%s: While loading data type subsystem some issues was found. Please fix these issues."), ANSI_TO_TCHAR(__FUNCTION__));
}

bool UDataTypeSubsystem::IsValidDataType(const FDataTypeBase& InDataTypeId)
{
	if (AllDataTypes.Contains(InDataTypeId))
	{
		return true;
	}

	return false;
}

void UDataTypeSubsystem::Editor_UpdateTypeLoadState(bool bSuccess, const UScriptStruct* InTypeStructClass)
{
#if WITH_EDITOR
	const FString& InTypeStructName = InTypeStructClass->GetName();
	if (!DataTypeLoadState.Contains(InTypeStructName))
	{
		DataTypeLoadState.Add(InTypeStructName, bSuccess);
	}

	if (!bSuccess)
	{
		bInitialised = false;
		const bool bCurrentLoadState = DataTypeLoadState.FindRef(InTypeStructName);
		if (bCurrentLoadState)
		{
			DataTypeLoadState.Emplace(InTypeStructName, false);
		}
	}
#endif
}

#if WITH_EDITOR
void UDataTypeSubsystem::Editor_DataTableModified()
{
	Editor_MarkNeedsRefresh();
}

void UDataTypeSubsystem::Editor_MarkNeedsRefresh()
{
	if (bNeedsRefresh)
	{
		return;
	}
	
	const FText NotificationText = FText::FromString("Data types subsystem needs reloading to apply changes.");
	FNotificationInfo RefreshMsg = FNotificationInfo(NotificationText);
	RefreshMsg.SubText = FText::FromString("Reload the data type subsystem by restarting the editor, or by pressing the Refresh Data Types button in the Type Toolbox settings in the editor toolbar...");
	RefreshMsg.bFireAndForget = false;
	RefreshMsg.bUseSuccessFailIcons = false;
	RefreshNotificationPtr = FSlateNotificationManager::Get().AddNotification(RefreshMsg);
	bNeedsRefresh = true;
}

void UDataTypeSubsystem::Editor_Refreshed()
{
	if (RefreshNotificationPtr.IsValid())
	{
		RefreshNotificationPtr->SetCompletionState(SNotificationItem::CS_Success);
		RefreshNotificationPtr->Fadeout();
		RefreshNotificationPtr = nullptr;
	}
	
	bNeedsRefresh = false;
}
#endif