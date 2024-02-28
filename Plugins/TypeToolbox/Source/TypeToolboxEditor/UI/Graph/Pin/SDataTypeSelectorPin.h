// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "TypeToolbox/DataType/BaseDataType.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SDataTypeComboBox;

class SDataTypeSelectorPin : public SGraphPin
{
	
public:

	SLATE_BEGIN_ARGS(SDataTypeSelectorPin)
	: _SelectedStructType(nullptr)
	{}
	SLATE_ARGUMENT(UScriptStruct*, SelectedStructType)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
	
	/** Override label widget to change the visibility function. */
	virtual TSharedRef<SWidget> GetLabelWidget(const FName& InLabelStyle) override;

	/** Returns the label widgets current text value... */
	FText GetLabelText() const;

	/** This override is used to display slate widget used for customization. */
	virtual TSharedRef<SWidget> GetDefaultValueWidget() override;
	
	/** Returns the current value for the FDataTypeBase this pin represents. */
	FDataTypeBase GetCurrentDataTypeValue() const;

private:
	
	/** Returns visibility for the combo box based on ShouldShowDefaultWidget(). */
	EVisibility GetComboBoxVisibility() const;
	
	/** Returns visibility for the label based on ShouldShowDefaultWidget(). */
	EVisibility GetPinLabelCustomVisibility() const;

	/** Return current set value as text derived from the FDataTypeBase structure row name. */
	FText GetDataValueComboTextValue() const;

	/** If the pin has any connections we should show the default pin widget... */
	bool ShouldShowDefaultWidget() const;

	/** Clear the current value of the graph pin... */
	void ClearDataTypeValue() const;

	/** Called when a row is selected out of the combo box. */
	void DataTypeSelected(const FDataTypeBase SelectedDataType, const FString SelectedRowName) const;

	/** The current selected data type struct from the DataTypeStructPickerDropdown. */
	UScriptStruct* SelectedDataTypeStruct = nullptr;

	/** Data type Id selector dropdown combo button. */
	TSharedPtr<SDataTypeComboBox> DataTypeComboButton = nullptr;
};
