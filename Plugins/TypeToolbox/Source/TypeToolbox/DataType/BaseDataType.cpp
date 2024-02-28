// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDataType.h"
#include "TypeToolboxLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogDataTypeBase, Log, All);

bool FDataTypeBase::IsValid(EDataTypeValid ValidCheck) const
{
	switch (ValidCheck)
	{
	case EDataTypeValid::Default:
		return !(RowName.IsEqual(InvalDataTypeRowName) || DataTableIndex < 0) && UTypeToolboxLibrary::IsDataTypeValid(*this);
		
	case EDataTypeValid::ExistsInDataBase:
		return UTypeToolboxLibrary::IsDataTypeValid(*this);
		
	case EDataTypeValid::NotMissingData:
		return !(RowName.IsEqual(InvalDataTypeRowName) || DataTableIndex < 0);

	case EDataTypeValid::NotEmpty:
		return !(RowName.IsEqual(InvalDataTypeRowName) && DataTableIndex < 0);
		
	default:
		return true;
	}
}

bool FDataTypeBase::Serialize(FArchive& Ar)
{
	Ar << RowName;
	Ar << DataTableIndex;
	return true;
}

bool FDataTypeBase::ExportTextItem(FString& ValueStr, FDataTypeBase const& PropertyValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
	ValueStr = FString::Printf(TEXT("FDataTypeBase(RowName=%s, DataTableIndex=%s)"), *RowName.ToString(), *FString::FromInt(DataTableIndex));
	return true;
}

bool FDataTypeBase::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* OwnerObject, FOutputDevice* ErrorText)
{
	const FString StringBuffer = FString(Buffer);
	if (StringBuffer.IsEmpty())
	{
		return false;
	}

	// Extract the row name.
	int32 RowNamePos = StringBuffer.Find(TEXT("RowName="));
	if (RowNamePos != INDEX_NONE)
	{
		RowNamePos += FString(TEXT("RowName=")).Len();
		const int32 CommaPos = StringBuffer.Find(TEXT(","), ESearchCase::CaseSensitive, ESearchDir::FromStart, RowNamePos);
		const FString RowNameValue = StringBuffer.Mid(RowNamePos, CommaPos - RowNamePos);
		RowName = FName(*RowNameValue);
	}
	else
	{
		return false;
	}
	
	// Extract the data table id.
	int32 DataTableIndexPos = StringBuffer.Find(TEXT("DataTableIndex="));
	if (DataTableIndexPos != INDEX_NONE)
	{
		DataTableIndexPos += FString(TEXT("DataTableIndex=")).Len();
		const int32 ClosingBracketPos = StringBuffer.Find(TEXT(")"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		const FString DataTableIndexValue = StringBuffer.Mid(DataTableIndexPos, ClosingBracketPos - DataTableIndexPos);
		DataTableIndex = FCString::Atoi(*DataTableIndexValue);
	}
	else
	{
		return false;
	}
	
	return true;
}

bool FDataTypeBase::IsValidForBuild() const
{
	const int32 StatusMask = UTypeToolboxLibrary::GetStatusMaskForValue(*this);
		
	// If depreciated its not valid.
	if (EnumHasAnyFlags(EDataTypeStatus(StatusMask), EDataTypeStatus::Depreciated))
	{
		return false;
	}
		
#if !UE_BUILD_SHIPPING
	if (EnumHasAnyFlags(EDataTypeStatus(StatusMask), EDataTypeStatus::Development))
	{
		return true;
	}
	else if (EnumHasAnyFlags(EDataTypeStatus(StatusMask), EDataTypeStatus::Live))
	{
		return true;
	}
#else
	if (EnumHasAnyFlags(EDataTypeStatus(StatusMask), EDataTypeStatus::Live))
	{
		return true;
	}
#endif

	return false;
}


