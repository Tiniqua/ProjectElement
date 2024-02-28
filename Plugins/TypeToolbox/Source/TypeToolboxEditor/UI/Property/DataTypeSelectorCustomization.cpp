// Fill out your copyright notice in the Description page of Project Settings.

#include "DataTypeSelectorCustomization.h"
#include "DetailWidgetRow.h"
#include "TypeToolbox/DataType/DataTypeProperties.h"
#include "TypeToolboxEditor/UI/Widgets/SStructComboBox.h"
#include "TypeToolboxEditor/Utility/TypeToolboxUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogDataTypeSelector, Log, All);

void FDataTypeSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;
	check(StructPropertyHandle.IsValid());
	
	// Set the header row name to be the default structure name
	// Then add a drop down menu for seeing all types of structures.
	HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()]
	.ValueContent()[
		SNew(SBox)
		.WidthOverride(280)
		[
			SAssignNew(StructPickerDropdown, SStructComboBox)
			.ContentPadding(FMargin(2,2,2,1))
			.MenuPlacement(MenuPlacement_BelowAnchor)
			.StructViewFilter(MakeShared<FDataTypeBaseStructFilter>())
			.OnStructPicked(FOnStructPicked::CreateRaw(this, &FDataTypeSelectorCustomization::OnStructPicked))
			.ButtonContentText(this, &FDataTypeSelectorCustomization::GetComboTextValue)
		]
	];
}

void FDataTypeSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//...
}

FText FDataTypeSelectorCustomization::GetComboTextValue() const
{
	check(StructPropertyHandle.IsValid());
	
	void* PropertyData;
	StructPropertyHandle->GetValueData(PropertyData);
	if (!PropertyData)
	{
		return FText::FromString("None");
	}
	
	const FDataTypeSelector DataTypeSelectorValue = *static_cast<FDataTypeSelector*>(PropertyData);
	if (!DataTypeSelectorValue.SelectedStructType)
	{
		return FText::FromString("None");
	}

	return FText::AsCultureInvariant(DataTypeSelectorValue.SelectedStructType->GetName());
}

void FDataTypeSelectorCustomization::OnStructPicked(const UScriptStruct* PickedStruct) const
{
	check(StructPropertyHandle.IsValid());

	void* PropertyData;
	StructPropertyHandle->GetValueData(PropertyData);
	if (!PropertyData)
	{
		UE_LOG(LogDataTypeSelector, Error, TEXT("%s: Cannot set structure as the struct property handle has no data on the current structure!"), ANSI_TO_TCHAR(__FUNCTION__));
		StructPickerDropdown->SetIsOpen(false);
		return;
	}
	
	FDataTypeSelector* DataTypeSelectorValue = static_cast<FDataTypeSelector*>(PropertyData);
	if (!DataTypeSelectorValue)
	{
		UE_LOG(LogDataTypeSelector, Error, TEXT("%s: Cannot set structure as the struct data cannot be converted to FDataTypeSelector!"), ANSI_TO_TCHAR(__FUNCTION__));
		StructPickerDropdown->SetIsOpen(false);
		return;
	}
	
	DataTypeSelectorValue->SelectedStructType = const_cast<UScriptStruct*>(PickedStruct);
	StructPickerDropdown->SetIsOpen(false);
	StructPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet); // Notifies the value change and runs PostEditChangeProperty...
}