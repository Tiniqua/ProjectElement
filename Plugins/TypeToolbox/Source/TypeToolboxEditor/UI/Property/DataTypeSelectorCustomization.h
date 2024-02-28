// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

struct FTableRowSelector;
class SStructComboBox;

/** Custom property type to provide a drop down list of all types of FDataTypeBaseId child structures for selection. */
class FDataTypeSelectorCustomization : public IPropertyTypeCustomization
{
	
public:
	
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FDataTypeSelectorCustomization());
	}

	// Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	// End IPropertyTypeCustomization interface

	/** Ran when an item / struct is picked from the StructPickerDropdown. */
	void OnStructPicked(const UScriptStruct* PickedStruct) const;

	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	
private:

	/** Return current set value as text. */
	FText GetComboTextValue() const;

	/** Struct dropdown combo button. */
	TSharedPtr<SStructComboBox> StructPickerDropdown;
};
