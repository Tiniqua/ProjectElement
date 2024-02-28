// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "TypeToolbox/DataType/BaseDataType.h"

class SDataTypeComboBox;

/** Custom property drop down menu that uses the UDataTypeSubsystem to fill in a given type as an option.
 * 	The selection will store values for the Id (Row name) and the database id held in a list inside the
 * 	UDataTypeSubsystem.
 */
class FDataTypeBaseCustomization : public IPropertyTypeCustomization
{
	
public:
	
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FDataTypeBaseCustomization());
	}

	// Begin IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	// End IPropertyTypeCustomization interface
	
	TSharedPtr<IPropertyHandle> StructPropertyHandle;

protected:

	/** Called when a row is selected out of the combo box. */
	void DataTypeSelected(const FDataTypeBase SelectedDataType, const FString SelectedRowName) const;

	/** Returns the text value of the set item in the combo box from the current value of this property. */
	FText GetComboTextValue() const;

	/** Returns the current FDataTypeBaseId* ptr value for this property. */
	FDataTypeBase* GetCurrentValuePtr() const;

	/** Returns the current static struct type for this property. */
	UScriptStruct* GetScriptStruct() const;

	/** Data type Id selector dropdown combo button. */
	TSharedPtr<SDataTypeComboBox> DataTypeComboButton = nullptr;
};
