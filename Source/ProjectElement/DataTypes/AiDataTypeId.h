
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeToolbox/DataType/BaseDataType.h"
#include "AiDataTypeId.generated.h"

USTRUCT(BlueprintType)
struct FAiDataType : public FDataTypeBase
{
	GENERATED_BODY()

	using FDataTypeBase::FDataTypeBase;
	using FDataTypeBase::operator=;
	using FDataTypeBase::operator==;
	using FDataTypeBase::operator!=;
};

template<>
struct TStructOpsTypeTraits<FAiDataType> : public TStructOpsTypeTraitsBase2<FAiDataType>
{
	enum
	{
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};

class AElementAiCharacter;

USTRUCT(BlueprintType)
struct FAiTypeTableRow : public FTypeTableRowBase
{
	GENERATED_BODY()

	/** AI Character class. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI Character")
	TSubclassOf<AElementAiCharacter> AiCharacterClass = nullptr;

	// Etc.
};

/**
 *  NOTE: Here's an example of how you can access these types if correctly defined in the Type Toolbox project settings.

	UDataTable* AiTypeDataTable = nullptr;
	DataTypeSubsystem->GetDataTableFromStruct(AiTypeDataTable, FAiDataTypeId::StaticStruct());

	FAiTypeTableRow LookForThisId;
	const FAiTypeTableRow* TableRow = AiTypeDataTable->FindRow<FAiTypeTableRow>(LookForThisId.Id, "FindAiTypeTableRow");
	if (!TableRow)
	{
		return;
	}

	TableRow->AiCharacterClass etc...
*/