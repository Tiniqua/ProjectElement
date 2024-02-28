// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BaseDataType.h"
#include "DataTypeProperties.h"
#include "Subsystems/EngineSubsystem.h"
#include "DataTypeSubsystem.generated.h"

class UDataTable;

DEFINE_LOG_CATEGORY_STATIC(LogDataTypeSubsystem, Log, All);

/**
 * Subsystem used to initialise and declare databases with subtypes that can be accessed from anywhere.
 * NOTE: Make sure Data table folder is added to additional directories to cook and include in build as they are soft referenced.
 */
UCLASS()
class TYPETOOLBOX_API UDataTypeSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	
	// Begin USubsystem Override
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem Override

	/** Reset the subsystem and re-initialise. */
	void MarkSubsystemDirty();
	
	/** Returns list of the active data type definitions from the type toolbox settings.
	 *  Key: Type of FDataTypeBaseId this type is. (e.g FDataTypeBase etc.)
	 *  Value: FDataTypeSettings, the type of row to use for this type and a list of active data tables for this type. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	TMap<FDataTypeSelector, FDataTypeSettings> GetDataTypesSettings() const
	{
		return DataTypeSettings;
	}
	
	/** Returns list of all loaded data tables. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	TArray<UDataTable*> GetLoadedDataTables() const
	{
		return LoadedDataTables;
	}

	/** Returns mapping of all type row names to a data table.
	 *  Key: Row Name of a given type (e.g. Type_Potion_Heal)
	 *  Value: Data table that owns this type. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	TMap<FString, UDataTable*> GetRowNameToTableMap() const
	{
		return RowNameToTableMap;
	}

	/** Returns mapping of all types of all loaded data type structures to their data tables.
	 *  Key: The status struct of this type. (e.g. FPotionType)
	 *  Value: Data table that owns this type. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	TMap<UScriptStruct*, UDataTable*> GetStructToTableMap() const
	{
		return StructToTableMap;
	}

	/** Returns List of all possible configurations of FDataTypeBase that are currently loaded into the subsystem. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	TSet<FDataTypeBase> GetAllDataTypeValues() const
	{
		return AllDataTypes;
	}

	/** Find data table index from the data table. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	int32 GetDataTableIndex(const UDataTable* InDataTable) const
	{
		for (int32 i = 0; i < LoadedDataTables.Num(); i++)
		{
			if (LoadedDataTables[i] == InDataTable)
			{
				return i;
			}
		}

		return -1;
	}

	/** Returns true or false if the data type is valid or not. */
	UFUNCTION(BlueprintCallable, Category = "DataType")
	bool IsValidDataType(const FDataTypeBase& InDataTypeId);

#if WITH_EDITOR
	
	UFUNCTION()
	void Editor_DataTableModified();
	
	void Editor_MarkNeedsRefresh();
	void Editor_Refreshed();
	
#endif
	
#if WITH_EDITORONLY_DATA
	
	bool bNeedsRefresh = false;
	
	/** Loaded state of a given data type for visibility on any errors to users in editor time.
	 *  Key: Data type structure name.
	 *  Value: Boolean, true if successfully loaded, false if errors was detected. */
	UPROPERTY(BlueprintReadOnly, Category = "DataType")
	TMap<FString, bool> DataTypeLoadState;

#endif

	bool bInitialised = false;
	
protected:
	
	/** Reset the subsystem. */
	void Cleanup();
	
	/** Initialise the subsystem. */
	void Setup();

	/** Update DataTypeLoadState for each type after each step of validation. */
	void Editor_UpdateTypeLoadState(bool bSuccess, const UScriptStruct* InTypeStructClass);

	/** Cached list of the active data type definitions from the type toolbox settings.
	 *  Key: Type of FDataTypeBaseId this type is. (e.g FDataTypeBase etc.)
	 *  Value: FDataTypeSettings, the type of row to use for this type and a list of active data tables for this type. */
	UPROPERTY(BlueprintReadOnly, Category = "General")
	TMap<FDataTypeSelector, FDataTypeSettings> DataTypeSettings;
	
	/** Hard reference to all of the data tables to keep them in
	 *  memory during the scope of this application. */
	UPROPERTY(BlueprintReadOnly, Category = "DataType")
	TArray<UDataTable*> LoadedDataTables;

	/** Map for mapping all type row names to a data table.
	 *  Key: Row Name of a given type (e.g. Type_Potion_Heal)
	 *  Value: Data table that owns this type. */
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UDataTable*> RowNameToTableMap;

	/** Map for mapping all types of all loaded data type structures to their data tables.
  	 *  Key: The status struct of this type. (e.g. FPotionType)
  	 *  Value: Data table that owns this type. */
	UPROPERTY(BlueprintReadOnly)
	TMap<UScriptStruct*, UDataTable*> StructToTableMap;

	/** List of all possible configurations of FDataTypeBase that are currently loaded into the subsystem. */
	UPROPERTY(BlueprintReadOnly, Category = "DataType")
	TSet<FDataTypeBase> AllDataTypes;

private:

#if WITH_EDITOR
	
	TSharedPtr<SNotificationItem> RefreshNotificationPtr = nullptr;
	
#endif
};
