// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "StructViewerModule.h"

class IStructViewerFilter;
class SComboButton;

class SStructComboBox : public SCompoundWidget
{
	
public:

	/** Slate widget input params. */
	SLATE_BEGIN_ARGS(SStructComboBox)
		: _MenuPlacement(MenuPlacement_ComboBox)
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Center)
		, _StructViewFilter(nullptr)
		, _ButtonContentText(FText::FromString("None"))
	{}
	
	SLATE_NAMED_SLOT(FArguments, ButtonContent)
	
	/** Sets an event handler to generate a widget dynamically when the menu is needed. */
	SLATE_EVENT(FOnGetContent, OnGetMenuContent)
	SLATE_EVENT(FOnIsOpenChanged, OnMenuOpenChanged)
	SLATE_EVENT(FOnComboBoxOpened, OnComboBoxOpened)
	SLATE_EVENT(FOnStructPicked, OnStructPicked)

	SLATE_ATTRIBUTE(FMargin, ContentPadding)
	SLATE_ATTRIBUTE(EMenuPlacement, MenuPlacement)
	
	SLATE_ARGUMENT(EHorizontalAlignment, HAlign)
	SLATE_ARGUMENT(EVerticalAlignment, VAlign)
	SLATE_ARGUMENT(TSharedPtr<IStructViewerFilter>, StructViewFilter)
		
	SLATE_ATTRIBUTE(FText, ButtonContentText)

	SLATE_END_ARGS()

	/** Construct the widget from a declaration. */
	void Construct(const FArguments& InArgs);

	/** Open or close the dropdown menu. */
	void SetIsOpen(bool InIsOpen, const bool bFocusMenu = true, const int32 FocusUserIndex = 0) const;
	
	/** Return true if the dropdown is open; false otherwise. */
	bool IsOpen() const;
	
protected:
	
	/** Ptr to the struct picker combo box. */
	TSharedPtr<SComboButton> StructPickerDropdown = nullptr;

	/** Selected struct filter view for filling the combo box with options. */
	TSharedPtr<IStructViewerFilter> CurrentStructViewFilter = nullptr;
};
