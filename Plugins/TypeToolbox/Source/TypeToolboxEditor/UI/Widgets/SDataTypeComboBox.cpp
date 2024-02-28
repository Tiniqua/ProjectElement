// Fill out your copyright notice in the Description page of Project Settings.

#include "SDataTypeComboBox.h"
#include "TypeToolbox/DataType/DataTypeSubsystem.h"
#include "TypeToolbox/DataType/TypeToolboxLibrary.h"
#include "Widgets/Input/SSearchBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogDataTypeEditorWidget, Log, All);

void SDataTypeComboBox::Construct(const FArguments& InArgs)
{
	DataTypeStaticStruct = InArgs._DataTypeStaticStruct;
	check(DataTypeStaticStruct);

	// Store delegates.
	EventSearchTextChanged = InArgs._OnSearchTextChanged;
	EventDataTypeSelected = InArgs._OnDataTypeSelected;
	
	// Update initial selected row name by default.
	SelectedRowName = MakeShared<FString>(InArgs._InitialSelectedRowName);

	// Find and store data table.
	UDataTable* FoundDataTable = UTypeToolboxLibrary::GetDataTableFromTypeStruct(DataTypeStaticStruct);
	if (!FoundDataTable)
	{
		// If we cannot find it print message as widget.
		ChildSlot
		[
			SNew(SBox)
			.WidthOverride(280)
			[
				SNew(STextBlock)
				.Text(FText::FromString("NO DATA TABLE FOUND! Error..."))
			]
		];

		UE_LOG(LogDataTypeEditorWidget, Error, TEXT("%s: Cannot find the Data Table For this struct to set the properties?!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	TypeDataTable = FoundDataTable;
	GenerateRowNames();
	UpdateSelectedDataTypeValue();

	SAssignNew(DataTypeRowNameDropdown, SComboButton)
	.ToolTipText(InArgs._ToolTipText)
	.ContentPadding(InArgs._ContentPadding)
	.MenuPlacement(InArgs._MenuPlacement)
	.HAlign(InArgs._HAlign)
	.VAlign(InArgs._VAlign)
	.Visibility(InArgs._Visibility)
	.ButtonContent()
	[
		SNew(STextBlock)
		.Text(InArgs._ButtonContentText)
		.ColorAndOpacity(this, &SDataTypeComboBox::IsDataTypeValidColor)
	]
	.MenuContent()
	[
	    SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(DataTypeSearchBox, SSearchBox)
			.OnTextChanged(this, &SDataTypeComboBox::OnSearchTextChanged)
			.MinDesiredWidth(250)
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(DropdownContentList, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&RowNames)
			.OnSelectionChanged(this, &SDataTypeComboBox::OnDataTypeRowSelected)
			.OnGenerateRow(this, &SDataTypeComboBox::GenerateDataTypeRow)
			.SelectionMode(ESelectionMode::Single)
		]
	]
	.OnMenuOpenChanged(InArgs._OnMenuOpenChanged)
	.OnComboBoxOpened(InArgs._OnComboBoxOpened);

	OnSearchTextChanged(FText::GetEmpty());
	if (SelectedRowName.IsValid())
	{
		DropdownContentList->SetSelection(SelectedRowName);
	}
	
	ChildSlot
	[
		DataTypeRowNameDropdown.ToSharedRef()
	];
}

FString SDataTypeComboBox::GetSelectedRowName() const
{
	check(SelectedRowName.IsValid());
	return *SelectedRowName;
}

void SDataTypeComboBox::UpdateSelectedDataTypeValue()
{
	if (!SelectedRowName.IsValid())
	{
		return;
	}

	if (!SelectedDataTypeValue.IsValid())
	{
		SelectedDataTypeValue = MakeShared<FDataTypeBase>();
	}

	// If none, default the struct for FDataTypeBase.
	if (*SelectedRowName == "None")
	{
		SelectedDataTypeValue->ClearType();
		return;
	}

	// Otherwise attempt to find the data table index. NOTE: If it is not found it will have partial data and show up as red text...
	SelectedDataTypeValue->RowName = FName(*SelectedRowName);
	const UDataTable* FoundTable = UTypeToolboxLibrary::GetDataTableFromTypeRowName(*SelectedRowName);
	if (!FoundTable)
	{
		return;
	}
	
	const int32 FoundIndex = UTypeToolboxLibrary::GetDataTableIndex(FoundTable);
	if (FoundIndex < 0)
	{
		UE_LOG(LogDataTypeEditorWidget, Error, TEXT("%s: Cannot find the data table index in UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	SelectedDataTypeValue->DataTableIndex = FoundIndex;
}

FDataTypeBase SDataTypeComboBox::GetSelectedDataTypeValue() const
{
	if (!SelectedDataTypeValue.IsValid())
	{
		return FDataTypeBase();
	}
	
	return *SelectedDataTypeValue;
}

FSlateColor SDataTypeComboBox::IsDataTypeValidColor() const
{
	check(SelectedDataTypeValue.IsValid());
	FTextBlockStyle OriginalStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText");

	// Only show red text when the data type doesnt exist in the data base or is missing data.
	// When completely empty allow normal text. (This would be when None is selected) E.g FDataType(InvalidText, -1)...
	if (!SelectedDataTypeValue->IsValid(EDataTypeValid::NotEmpty) ||
		SelectedDataTypeValue->IsValid(EDataTypeValid::ExistsInDataBase))
	{
		return OriginalStyle.ColorAndOpacity;
	}

	return FSlateColor(FLinearColor::Red);
}

void SDataTypeComboBox::OnSearchTextChanged(const FText& TextInput)
{
	GenerateRowNames(TextInput.ToString());
	EventSearchTextChanged.ExecuteIfBound(TextInput);
}

void SDataTypeComboBox::GenerateRowNames(const FString& SearchInput)
{
	check(TypeDataTable);
	RowNames.Empty();

	if (!TypeDataTable)
	{
		RowNames.Empty();
		return;
	}

	const TArray<FName> FoundRowNames = TypeDataTable->GetRowNames();
	if (FoundRowNames.Num() <= 0)
	{
		RowNames.Empty();
		return;	
	}

	RowNames.Add(MakeShared<FString>("None"));
	for (const FName& Name : FoundRowNames)
	{
		// Check name against search input before adding...
		if (!SearchInput.IsEmpty())
		{
			const FString& CurrentRowString = Name.ToString();
			if (!CurrentRowString.Contains(SearchInput))
			{
				continue;
			}
		}
		
		RowNames.Add(MakeShared<FString>(Name.ToString()));
	}
}

void SDataTypeComboBox::OnDataTypeRowSelected(TSharedPtr<FString> RowName, ESelectInfo::Type SelectInfo)
{
	if (SelectInfo != ESelectInfo::Type::OnMouseClick)
	{
		return;
	}

	// Close and set combo box.
	DataTypeRowNameDropdown->SetIsOpen(false);
	SelectedRowName = RowName;
	UpdateSelectedDataTypeValue();

	// Broadcast new selection.
	EventDataTypeSelected.ExecuteIfBound(*SelectedDataTypeValue, *SelectedRowName);
}

TSharedRef<ITableRow> SDataTypeComboBox::GenerateDataTypeRow(TSharedPtr<FString> RowName, const TSharedRef<STableViewBase>& OwningTable) const
{
	return SNew(STableRow<TSharedPtr<FString>>, OwningTable)
	[
		SNew(STextBlock).Text(FText::FromString(*RowName))
		.MinDesiredWidth(250)
	];
}