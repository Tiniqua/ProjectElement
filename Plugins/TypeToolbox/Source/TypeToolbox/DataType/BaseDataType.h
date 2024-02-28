// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseDataType.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogDataType, Log, All);

inline static const FName InvalDataTypeRowName = FName("Type_Invalid");

/** Status mask types for any data types that will determine what types of builds they make it into. */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDataTypeStatus : uint8
{
	Empty = 0 UMETA(Hidden), // Empty, ignored.
	Development = 1 << 0,	 // Should only be included in non-shipping builds.
	Depreciated = 1 << 1,    // Should be cut from all builds.
	Live = 1 << 2			 // Ready for use in fully shipped game.
};
ENUM_CLASS_FLAGS(EDataTypeStatus);

UENUM(BlueprintType)
enum class EDataTypeValid : uint8
{
	Default,		     // Will check both the existence in the data type subsystem and that is it not empty / invalid.
	ExistsInDataBase,    // Will check if this data type exists within the data type subsystem.
	NotMissingData,		 // Will check if the row name or data table index is invalid
	NotEmpty		     // Will check if this data type is empty.
};

/** Base data type with support for replication and valid checks depending on its status mask.
 *  RowName: Represents the row name for this data type found from a valid type data table initialised in the type data base.
 *  DataTableIndex: Index location of the data table this data type belongs to in the UDataTypeSubsystem::LoadedDataTables.
 *
 *  NOTE: HasNativeMake is here to override the BP split pin functionality so only data type values that exist within the
 *  UDataTypeSubsystem can be used...
 **/
USTRUCT(BlueprintType, meta = (HasNativeMake = ""))
struct TYPETOOLBOX_API FDataTypeBase
{
	GENERATED_BODY()

	/** Name of this data type, assigned after instanced from the data types data tables. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataType")
	FName RowName;
	
	/** Name of this data type, assigned after instanced from the data types data tables. */
	UPROPERTY(BlueprintReadOnly, Category = "DataType")
	int32 DataTableIndex;
	
	FDataTypeBase() : RowName(InvalDataTypeRowName), DataTableIndex(-1) {}
	FDataTypeBase(const FDataTypeBase& InDataType, const int32 InDataTableIndex) : RowName(InDataType.RowName), DataTableIndex(InDataTableIndex) {}
	FDataTypeBase(const FName& InName, const int32 InDataTableIndex) : RowName(InName), DataTableIndex(InDataTableIndex) {}
	
	/** Return this data type name as a string. */
	FString RowNameAsString() const
	{
		return RowName.ToString();
	}

	/** Clear this data type value back to default. */
	void ClearType()
	{
		RowName = InvalDataTypeRowName;
		DataTableIndex = -1;
	}

	/** Is this data type valid / filled with a given RowName and DatabaseIndex. Also check existence in the data type subsystem. */
	bool IsValid(EDataTypeValid ValidCheck = EDataTypeValid::Default) const;
	
	bool Serialize(FArchive& Ar);

	bool ExportTextItem(FString& ValueStr, FDataTypeBase const& PropertyValue, class UObject* Parent, int32 PortFlags, class UObject* ExportRootScope) const;

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, class UObject* OwnerObject, FOutputDevice* ErrorText);
	
	/** Returns if this data type is valid for a given build. This can be ran on the value before usage to make sure.
	 *  NOTE: Has to be done per use case for status masks to be effective. */
	bool IsValidForBuild() const;

	bool operator==(const FDataTypeBase& Other) const
	{
		return RowName.IsEqual(Other.RowName) && DataTableIndex == Other.DataTableIndex;
	}

	bool operator!=(const FDataTypeBase& Other) const
	{
		return !RowName.IsEqual(Other.RowName) || DataTableIndex != Other.DataTableIndex;
	}

	FDataTypeBase& operator=(const FDataTypeBase& Other)
	{
		RowName = Other.RowName;
		DataTableIndex = Other.DataTableIndex;
		return *this;
	}
};

template<>
struct TStructOpsTypeTraits<FDataTypeBase> : public TStructOpsTypeTraitsBase2<FDataTypeBase>
{
	enum
	{
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};

inline uint32 GetTypeHash(const FDataTypeBase& InDataTypeBase)
{
	uint32 HashCode = GetTypeHash(InDataTypeBase.RowName);
	HashCode = HashCombine(HashCode, InDataTypeBase.DataTableIndex);
	return HashCode;
}

USTRUCT(BlueprintType)
struct FTypeUserInterfaceInfo
{
	GENERATED_BODY()
	
	/** Full display name for this data type to be displayed on UI. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FString DisplayName;

	/** Full description for this data type to be displayed on UI. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FString Description;
};

/** Defaults that are expected to be in all data type rows contained within their tables. */
USTRUCT(BlueprintType)
struct FTypeTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

	/** Status mask for this data type. (Should be included in build?) NOTE: Up to the user to use this. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "/Script/TypeToolbox.EDataTypeStatus"))
	int32 StatusMask = 0;

	/** UI information and settings for this specific data type entry. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FTypeUserInterfaceInfo UserInterfaceInfo;
};