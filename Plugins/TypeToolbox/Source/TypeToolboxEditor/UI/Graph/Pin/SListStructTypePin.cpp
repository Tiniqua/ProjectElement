// Fill out your copyright notice in the Description page of Project Settings.

#include "SListStructTypePin.h"
#include "StructViewerModule.h"
#include "TypeToolboxEditor/UI/Widgets/SStructComboBox.h"

void SListStructTypePin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	StructViewerFilter = InArgs._StructViewerFilter;
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget> SListStructTypePin::GetLabelWidget(const FName& InLabelStyle)
{
	return SNew(STextBlock)
	.Text(this, &SListStructTypePin::GetLabelText)
	.TextStyle(FAppStyle::Get(), InLabelStyle)
	.ColorAndOpacity(FLinearColor::White)
	.Visibility(this, &SListStructTypePin::GetPinLabelCustomVisibility);
}

FText SListStructTypePin::GetLabelText() const
{
	UEdGraphPin* GraphPin = GetPinObj();
	if (GraphPin && !GraphPin->IsPendingKill())
	{
		if (UEdGraphNode* GraphNode = GraphPin->GetOwningNodeUnchecked())
		{
			return GraphNode->GetPinDisplayName(GraphPin);
		}
	}
	
	return FText::GetEmpty();
}

TSharedRef<SWidget> SListStructTypePin::GetDefaultValueWidget()
{
	SAssignNew(StructPickerDropdown, SStructComboBox)
	.Visibility(this, &SListStructTypePin::GetComboBoxVisibility)
	.ContentPadding(FMargin(2,2,2,1))
	.MenuPlacement(MenuPlacement_BelowAnchor)
	.StructViewFilter(StructViewerFilter)
	.OnStructPicked(FOnStructPicked::CreateRaw(this, &SListStructTypePin::OnStructPicked))
	.ButtonContentText(this, &SListStructTypePin::GetComboTextValue);

	return StructPickerDropdown.ToSharedRef();
}

EVisibility SListStructTypePin::GetPinLabelCustomVisibility() const
{
	if (ShouldShowDefaultWidget())
	{
		return EVisibility::Visible;
	}
	
	return EVisibility::Collapsed;
}

EVisibility SListStructTypePin::GetComboBoxVisibility() const
{
	if (ShouldShowDefaultWidget())
	{
		if (PinImage->GetVisibility() != EVisibility::Visible)
		{
			PinImage->SetVisibility(EVisibility::Visible);
		}
		
		return EVisibility::Collapsed;
	}

	if (PinImage->GetVisibility() != EVisibility::Collapsed)
	{
		PinImage->SetVisibility(EVisibility::Collapsed);
	}
	
	return EVisibility::Visible;
}

bool SListStructTypePin::ShouldShowDefaultWidget() const
{
	if (GraphPinObj->Direction == EGPD_Output || GraphPinObj->HasAnyConnections())
	{
		return true;
	}
	
	return false;
}

FText SListStructTypePin::GetComboTextValue() const
{
	return FText::FromString("None");
}

void SListStructTypePin::OnStructPicked(const UScriptStruct* PickedStruct) const
{
	check(GraphPinObj)
	StructPickerDropdown->SetIsOpen(false);
}