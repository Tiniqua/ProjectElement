// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TypeToolbox/DataType/BaseDataType.h"

class UScriptStruct;
class SComboButton;
class UDataTable;

/** Notification when popup is opened/closed. */
DECLARE_DELEGATE_TwoParams(FDataTypeSelected, const FDataTypeBase, const FString);

/** Combo box with search box that will populate itself with the row options from a
 *  given data types data table loaded into the DataTypeSystem. */
class SDataTypeComboBox : public SCompoundWidget
{
	
public:

	/** Slate widget input params. */
	SLATE_BEGIN_ARGS(SDataTypeComboBox)
		: _OnSearchTextChanged()
		, _OnDataTypeSelected()
		, _MenuPlacement(MenuPlacement_ComboBox)
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Center)
		, _InitialSelectedRowName("None")
		, _DataTypeStaticStruct(nullptr)
		, _ButtonContentText(FText::FromString("None"))
	{}
	
	/** Sets an event handler to generate a widget dynamically when the menu is needed. */
	SLATE_EVENT(FOnGetContent, OnGetMenuContent)
	SLATE_EVENT(FOnIsOpenChanged, OnMenuOpenChanged)
	SLATE_EVENT(FOnComboBoxOpened, OnComboBoxOpened)
	SLATE_EVENT(FOnTextChanged, OnSearchTextChanged)
	SLATE_EVENT(FDataTypeSelected, OnDataTypeSelected)
	
	SLATE_ATTRIBUTE(FMargin, ContentPadding)
	SLATE_ATTRIBUTE(EMenuPlacement, MenuPlacement)
	
	SLATE_ARGUMENT(EHorizontalAlignment, HAlign)
	SLATE_ARGUMENT(EVerticalAlignment, VAlign)
	SLATE_ARGUMENT(FString, InitialSelectedRowName)
	SLATE_ARGUMENT(UScriptStruct*, DataTypeStaticStruct)
	
	SLATE_ATTRIBUTE(FText, ButtonContentText)

	SLATE_END_ARGS()

	/** Construct the widget from a declaration. */
	void Construct(const FArguments& InArgs);

	/** Returns selected row name if set, nullptr if empty. */
	FString GetSelectedRowName() const;
	
	/** Returns the current selected data type value in the base format FDataTypeBase. */
	FDataTypeBase GetSelectedDataTypeValue() const;

	/** Updates row names listed in combo box for selected data type value for row names. */
	void GenerateRowNames(const FString& SearchInput = "");
	
protected:

	/** Constructs the data type value from the SelectedRowName and the data table index
	 *  where that selected data type row name exists in the Data Type Subsystem. */
	void UpdateSelectedDataTypeValue();
	
	/** Returns green if the data type selected is valid, red if invalid. */
	FSlateColor IsDataTypeValidColor() const;

	/** Ran when the text filter on the DataTypeSearchBox is changed. */
	void OnSearchTextChanged(const FText& TextInput);

	/** Called when a row is selected out of the combo box. */
	void OnDataTypeRowSelected(TSharedPtr<FString> RowName, ESelectInfo::Type SelectInfo);
	
	/** Generate a row for the dropdown menu from the DropdownContentList. */
	TSharedRef<ITableRow> GenerateDataTypeRow(TSharedPtr<FString> RowName, const TSharedRef<STableViewBase>& OwningTable) const;

	/** The current selected data type value in the base format FDataTypeBase. */
	TSharedPtr<FDataTypeBase> SelectedDataTypeValue = nullptr;
	
	/** Current active data table for this type. */
	UDataTable* TypeDataTable = nullptr;

	/** Current active struct type for the data type. */
	UScriptStruct* DataTypeStaticStruct = nullptr;
	
	/** Data type Row name selector dropdown combo button. */
	TSharedPtr<SComboButton> DataTypeRowNameDropdown = nullptr;

	/** Current selected row name. */
	TSharedPtr<FString> SelectedRowName = nullptr;

	/** Row names being displayed depending on search string. */
	TArray<TSharedPtr<FString>> RowNames;

	/** List view for the menu in the dropdown box. */
	TSharedPtr<SListView<TSharedPtr<FString>>> DropdownContentList = nullptr;

	/** Search box for the data types in the drop down. */
	TSharedPtr<SSearchBox> DataTypeSearchBox = nullptr;

	FOnTextChanged EventSearchTextChanged;
	FDataTypeSelected EventDataTypeSelected;
};
