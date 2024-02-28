// Fill out your copyright notice in the Description page of Project Settings.

#include "DataTypeBaseCustomization.h"
#include "DetailWidgetRow.h"
#include "TypeToolbox/DataType/DataTypeSubsystem.h"
#include "TypeToolboxEditor/UI/Widgets/SDataTypeComboBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogDataTypeCustomization, Log, All);

void FDataTypeBaseCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;
	check(StructPropertyHandle.IsValid());

	// Update selected row from what the current set value is.
	FString CurrentSelection = "None";
	if (const FDataTypeBase* DataTypeValue = GetCurrentValuePtr())
	{
		CurrentSelection = DataTypeValue->RowName.ToString();
	}
	
	// Show drop down menu instead of original property.
	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()[
		SNew(SBox)
		.WidthOverride(280)
		[
			SAssignNew(DataTypeComboButton, SDataTypeComboBox)
			.ToolTipText(FText::FromString("Select a Data Type row name from the list."))
			.ContentPadding(FMargin(2,2,2,2))
			.MenuPlacement(MenuPlacement_BelowAnchor)
			.InitialSelectedRowName(CurrentSelection)
			.DataTypeStaticStruct(GetScriptStruct())
			.ButtonContentText(this, &FDataTypeBaseCustomization::GetComboTextValue)
			.OnDataTypeSelected(this, &FDataTypeBaseCustomization::DataTypeSelected)
		]
	];
}

void FDataTypeBaseCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//...
}

void FDataTypeBaseCustomization::DataTypeSelected(const FDataTypeBase SelectedDataType, const FString SelectedRowName) const
{
	FDataTypeBase* DataTypeValue = GetCurrentValuePtr();
	if (!DataTypeValue)
	{
		UE_LOG(LogDataTypeCustomization, Error, TEXT("%s: Cannot find the current DataTypeValue!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	if (SelectedRowName == "None")
	{
		DataTypeValue->ClearType();
		return;
	}
	
	DataTypeValue->RowName = SelectedDataType.RowName;
	DataTypeValue->DataTableIndex = SelectedDataType.DataTableIndex;
}

FText FDataTypeBaseCustomization::GetComboTextValue() const
{
	const FDataTypeBase* DataTypeValue = GetCurrentValuePtr();
	if (!DataTypeValue || !DataTypeValue->IsValid(EDataTypeValid::NotEmpty))
	{
		return FText::FromString("None");
	}
	
	const bool bValid = DataTypeValue->IsValid(EDataTypeValid::ExistsInDataBase);
	return bValid ? FText::FromName(DataTypeValue->RowName) : FText::FromString(DataTypeValue->RowName.ToString() + " (invalid)");
}

FDataTypeBase* FDataTypeBaseCustomization::GetCurrentValuePtr() const
{
	check(StructPropertyHandle.IsValid());
	
	void* PropertyData;
	StructPropertyHandle->GetValueData(PropertyData);
	if (!PropertyData)
	{
		return nullptr;
	}
	
	return static_cast<FDataTypeBase*>(PropertyData);
}

UScriptStruct* FDataTypeBaseCustomization::GetScriptStruct() const
{
	const UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogDataTypeCustomization, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}
	
	const FStructProperty* StructProperty = CastField<FStructProperty>(StructPropertyHandle->GetProperty());
	if (!StructProperty)
	{
		UE_LOG(LogDataTypeCustomization, Error, TEXT("%s: Cannot find struct property!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	UScriptStruct* CurrentStruct = StructProperty->Struct;
	if (!CurrentStruct)
	{
		UE_LOG(LogDataTypeCustomization, Error, TEXT("%s: Struct property is null!"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	return CurrentStruct;
}