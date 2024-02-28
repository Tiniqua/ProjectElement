// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTypeProperties.generated.h"

/** Custom drop down table row structure selector. */
USTRUCT(BlueprintType)
struct TYPETOOLBOX_API FTableRowSelector
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UScriptStruct* SelectedStructType;

	FTableRowSelector() : SelectedStructType(nullptr) {}
	FTableRowSelector(UScriptStruct* InStructType) : SelectedStructType(InStructType) {}
	
	bool operator==(const FTableRowSelector& Other) const
	{
		return SelectedStructType == Other.SelectedStructType;
	}
};

inline uint32 GetTypeHash(const FTableRowSelector& InTableRowSelector)
{
	uint32 Hash = FCrc::MemCrc32(&InTableRowSelector, sizeof(InTableRowSelector));
	return Hash;
}

/** Custom drop down base data type id selector. */
USTRUCT(BlueprintType)
struct TYPETOOLBOX_API FDataTypeSelector
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	UScriptStruct* SelectedStructType;

	FDataTypeSelector() : SelectedStructType(nullptr) {}
	FDataTypeSelector(const UScriptStruct* InStructType) : SelectedStructType(const_cast<UScriptStruct*>(InStructType)) {}

	bool operator==(const FDataTypeSelector& Other) const
	{
		return SelectedStructType == Other.SelectedStructType;
	}
};

inline uint32 GetTypeHash(const FDataTypeSelector& InDataTypeSelector)
{
	uint32 Hash = FCrc::MemCrc32(&InDataTypeSelector, sizeof(InDataTypeSelector));
	return Hash;
}

/** UI descriptive information about the data type that may or may not be needed depending on the use case. */
USTRUCT(BlueprintType)
struct TYPETOOLBOX_API FDataTypeUserInterfaceInfo
{
	GENERATED_BODY()
	
	/** Full name for this data type for UI if ever needed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DisplayName;

	/** Full description for this data type for UI if ever needed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Description;
};

/** Pair structure that reflects a data type string to its corresponding table type. */
USTRUCT(BlueprintType)
struct TYPETOOLBOX_API FDataTypeSettings
{
	GENERATED_BODY()

	/** This data types row name prefix that is validated during caching in the data type subsystem.
	 *  NOTE: This cannot be left blank as it ensures all type row definitions are unique across all data tables in the data type system.
	 *  Example: Type_Potion_ */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString TypeRowPrefix;
	
	/** Type of data table row to use for this type. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTableRowSelector DataTableRowType;
	
	/** Active database table for this data type.
	  * NOTE: If you wish to use more than one table just use a CDT. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> DatabaseTable;

	/** UI data for this data type. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FDataTypeUserInterfaceInfo UserInterfaceInfo;

	/** This will validate these settings are correctly setup then load and return the data table.
	 *  Returns true if valid false if not.
	 *  @Out OutLoadedDataTable: A loaded reference to the data table. 
	 *  @Out ErrorMsg: The error message of invalid setting found. Empty if none. */
	bool ValidateAndLoadDataTable(UDataTable*& OutLoadedDataTable, FString& OutErrorMsg) const
	{
		OutErrorMsg.Empty();
		OutLoadedDataTable = nullptr;

		// Check there is a row prefix set.
		if (TypeRowPrefix.IsEmpty())
		{
			OutErrorMsg = "TypeRowPrefix is empty! This must be set to keep every type definition unique!";
			return false;
		}
		
		// Check and load the data table.
		const FSoftObjectPath AssetPath = DatabaseTable.ToSoftObjectPath();
		UDataTable* LoadedTable = Cast<UDataTable>(AssetPath.TryLoad());
		if (!LoadedTable)
		{
			OutErrorMsg = FString::Printf(TEXT("DataTable at path: %s is not a valid UDataTable asset!"), (AssetPath.ToString().IsEmpty() ? *FString("Empty Path") : *AssetPath.ToString()));
			return false;
		}
		
		// Check there is an expected row structure type set for this type.
		UScriptStruct* SetRowStructure = DataTableRowType.SelectedStructType;
		if (!SetRowStructure)
		{
			OutErrorMsg = "DataTableRowType is not set!";
			return false;
		}

		// Check the data table is using the correct row for this type.
		if (SetRowStructure != LoadedTable->RowStruct)
		{
			OutErrorMsg = FString::Printf(TEXT("DataTable: %s for this type uses table row type %s when this type is set to use %s!"), *LoadedTable->GetName(), *LoadedTable->RowStruct->GetName(), *SetRowStructure->GetName());
			return false;
		}
		
		OutLoadedDataTable = LoadedTable;
		return true;
	}
};