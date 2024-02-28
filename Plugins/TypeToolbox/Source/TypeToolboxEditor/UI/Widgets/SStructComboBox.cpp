// Fill out your copyright notice in the Description page of Project Settings.

#include "SStructComboBox.h"

void SStructComboBox::Construct(const FArguments& InArgs)
{
	CurrentStructViewFilter = InArgs._StructViewFilter;
	check(CurrentStructViewFilter.IsValid());
	
	// Generate structure viewer options.	
	FStructViewerInitializationOptions Options;
	Options.Mode = EStructViewerMode::StructPicker;
	Options.StructFilter = CurrentStructViewFilter;
	Options.bShowUnloadedStructs = true;
	Options.bShowNoneOption = true;
	Options.NameTypeToDisplay = EStructViewerNameTypeToDisplay::StructName;
	Options.DisplayMode = EStructViewerDisplayMode::ListView;
	Options.bAllowViewOptions = false;
	
	// Create drop down for struct picker.
	SAssignNew(StructPickerDropdown, SComboButton)
	.HAlign(InArgs._HAlign)
	.VAlign(InArgs._VAlign)
	.Visibility(InArgs._Visibility)
	.ContentPadding(InArgs._ContentPadding)
	.MenuPlacement(InArgs._MenuPlacement)
	.ButtonContent()
	[
		SNew(STextBlock)
		.Text(InArgs._ButtonContentText)
	]
	.MenuContent()
	[
		SNew(SBox)
		.WidthOverride(280)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				FModuleManager::LoadModuleChecked<FStructViewerModule>("StructViewer").CreateStructViewer(Options, InArgs._OnStructPicked)
			]
		]
	]
	.OnMenuOpenChanged(InArgs._OnMenuOpenChanged)
	.OnComboBoxOpened(InArgs._OnComboBoxOpened);
	
	ChildSlot
	[
		StructPickerDropdown.ToSharedRef()
	];
}

void SStructComboBox::SetIsOpen(bool InIsOpen, const bool bFocusMenu, const int32 FocusUserIndex) const
{
	check(CurrentStructViewFilter.IsValid());
	StructPickerDropdown->SetIsOpen(InIsOpen, bFocusMenu, FocusUserIndex);
}

bool SStructComboBox::IsOpen() const
{
	check(CurrentStructViewFilter.IsValid());
	return StructPickerDropdown->IsOpen();
}