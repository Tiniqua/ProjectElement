// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SWidget;
class UEdGraphPin;
class UScriptStruct;
class IStructViewerFilter;
class SStructComboBox;

/** Code for generating a custom drop down list for a type of structure.
 *  There is SGraphPinStruct but this works differently and only supports input... */
class SListStructTypePin : public SGraphPin
{
	
public:
	
	SLATE_BEGIN_ARGS(SListStructTypePin)
	: _StructViewerFilter(nullptr)
	{}
	/** The type of struct view filter to use when filling combo box... */
	SLATE_ARGUMENT(TSharedPtr<IStructViewerFilter>, StructViewerFilter)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:

	/** Override label widget to change the visibility function. */
	virtual TSharedRef<SWidget> GetLabelWidget(const FName& InLabelStyle) override;

	/** Returns the label widgets current text value... */
	FText GetLabelText() const;

	/** This override is used to display slate widget used for customization. */
	virtual TSharedRef<SWidget> GetDefaultValueWidget() override;

	/** Returns visibility for the label based on ShouldShowDefaultWidget(). */
	EVisibility GetPinLabelCustomVisibility() const;
	
	/** Returns visibility for the combo box based on ShouldShowDefaultWidget(). */
	EVisibility GetComboBoxVisibility() const;

	/** Return current set value as text. */
	virtual FText GetComboTextValue() const;

	/** Ran when an item / struct is picked from the StructPickerDropdown. */
	virtual void OnStructPicked(const UScriptStruct* PickedStruct) const;

	/** Ptr to the struct picker combo box. */
	TSharedPtr<SStructComboBox> StructPickerDropdown = nullptr;
	
private:

	/** If the pin has any connections we should show the default pin widget... */
	bool ShouldShowDefaultWidget() const;
	
	TSharedPtr<IStructViewerFilter> StructViewerFilter = nullptr;
};
