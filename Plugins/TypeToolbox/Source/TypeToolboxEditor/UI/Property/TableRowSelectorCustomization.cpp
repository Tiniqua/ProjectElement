// Fill out your copyright notice in the Description page of Project Settings.

#include "TableRowSelectorCustomization.h"
#include "DetailWidgetRow.h"
#include "TypeToolboxEditor/Utility/TypeToolboxUtils.h"
#include "TypeToolbox/DataType/DataTypeProperties.h"
#include "TypeToolboxEditor/UI/Widgets/SStructComboBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogTableRowSelector, Log, All);

void FTableRowSelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
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
			.StructViewFilter(MakeShared<FDataTableStructFilter>())
			.OnStructPicked(FOnStructPicked::CreateRaw(this, &FTableRowSelectorCustomization::OnStructPicked))
			.ButtonContentText(this, &FTableRowSelectorCustomization::GetComboTextValue)
		]
	];
}

void FTableRowSelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//...
}

FText FTableRowSelectorCustomization::GetComboTextValue() const
{
	check(StructPropertyHandle.IsValid());
	
	void* PropertyData;
	StructPropertyHandle->GetValueData(PropertyData);
	if (!PropertyData)
	{
		return FText::FromString("None");
	}
	
	const FTableRowSelector TableRowSelectorValue = *static_cast<FTableRowSelector*>(PropertyData);
	if (!TableRowSelectorValue.SelectedStructType)
	{
		return FText::FromString("None");
	}

	return FText::AsCultureInvariant(TableRowSelectorValue.SelectedStructType->GetName());
}

void FTableRowSelectorCustomization::OnStructPicked(const UScriptStruct* PickedStruct) const
{
	check(StructPropertyHandle.IsValid());

	void* PropertyData;
	StructPropertyHandle->GetValueData(PropertyData);
	if (!PropertyData)
	{
		UE_LOG(LogTableRowSelector, Error, TEXT("%s: Cannot set structure as the struct property handle has no data on the current structure!"), ANSI_TO_TCHAR(__FUNCTION__));
		StructPickerDropdown->SetIsOpen(false);
		return;
	}
	
	FTableRowSelector* TableRowSelectorValue = static_cast<FTableRowSelector*>(PropertyData);
	if (!TableRowSelectorValue)
	{
		UE_LOG(LogTableRowSelector, Error, TEXT("%s: Cannot set structure as the struct data cannot be converted to FTableRowSelector!"), ANSI_TO_TCHAR(__FUNCTION__));
		StructPickerDropdown->SetIsOpen(false);
		return;
	}
	
	TableRowSelectorValue->SelectedStructType = const_cast<UScriptStruct*>(PickedStruct);
	StructPickerDropdown->SetIsOpen(false);
	StructPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet); // Notifies the value change and runs PostEditChangeProperty...
}