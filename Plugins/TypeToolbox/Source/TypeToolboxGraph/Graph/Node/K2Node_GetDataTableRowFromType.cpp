// Fill out your copyright notice in the Description page of Project Settings.

#include "K2Node_GetDataTableRowFromType.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "DataTableEditorUtils.h"
#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "K2Node_IfThenElse.h"
#include "KismetCompiler.h"
#include "TypeToolbox/DataType/BaseDataType.h"
#include "TypeToolbox/DataType/TypeToolboxLibrary.h"

#define LOCTEXT_NAMESPACE "UK2Node_GetDataTableRowFromType"

namespace GetDataTableRowHelper
{
	const FName DataTypeValuePinName = "DataTypeValue";
	const FName RowNotFoundPinName = "RowNotFound";
}

UK2Node_GetDataTableRowFromType::UK2Node_GetDataTableRowFromType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

void UK2Node_GetDataTableRowFromType::AllocateDefaultPins()
{
	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* RowFoundPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	RowFoundPin->PinFriendlyName = LOCTEXT("GetDataTableRow Row Found Exec pin", "Row Found");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetDataTableRowHelper::RowNotFoundPinName);

	// Add Data Type Value pin
	UEdGraphPin* DataTypeSelectorPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FDataTypeBase::StaticStruct(), GetDataTableRowHelper::DataTypeValuePinName);
	DataTypeSelectorPin->PinToolTip = LOCTEXT("DataTablePinDescription", "The DataTable you want to retreive a row from").ToString();
	
	// Result pin
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UEdGraphSchema_K2::PN_ReturnValue);
	ResultPin->PinFriendlyName = LOCTEXT("GetDataTableRow Output Row", "Out Row");
	ResultPin->PinToolTip = LOCTEXT("ResultPinDescription", "The returned TableRow, if found").ToString();
}

FText UK2Node_GetDataTableRowFromType::GetTooltipText() const
{
	return LOCTEXT("GetDataTableRowFromTypeTooltip", "Returns a valid row name of a given data type in its respective row type as a wildcard.");
}

FText UK2Node_GetDataTableRowFromType::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("GetDataTableRowFromType", "Get Data Table Row From Type");
}

FSlateIcon UK2Node_GetDataTableRowFromType::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_GetDataTableRowFromType::RefreshOutputPinType() const
{
	UScriptStruct* OutputType = GetDataTableRowStructType();
	SetReturnTypeForStruct(OutputType);
}

UScriptStruct* UK2Node_GetDataTableRowFromType::GetDataTableRowStructType() const
{
	UScriptStruct* RowStructType = nullptr;
	const UEdGraphPin* DataTypeValuePin = GetDataTypeValuePin();
	
	if(DataTypeValuePin && !DataTypeValuePin->DefaultValue.IsEmpty())
	{
		FDataTypeBase Result;
		FDataTypeBase::StaticStruct()->ImportText(*DataTypeValuePin->GetDefaultAsString(), &Result, nullptr, EPropertyPortFlags::PPF_None, GLog, FDataTypeBase::StaticStruct()->GetName());
		RowStructType = UTypeToolboxLibrary::GetDataTypeRowType(Result);
	}

	if (RowStructType == nullptr)
	{
		UEdGraphPin* ResultPin = GetResultPin();
		if (ResultPin && ResultPin->LinkedTo.Num() > 0)
		{
			RowStructType = Cast<UScriptStruct>(ResultPin->LinkedTo[0]->PinType.PinSubCategoryObject.Get());
			if (RowStructType == nullptr && ResultPin->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
			{
				RowStructType = GetFallbackStruct();
			}
			for (int32 LinkIndex = 1; LinkIndex < ResultPin->LinkedTo.Num(); ++LinkIndex)
			{
				const UEdGraphPin* Link = ResultPin->LinkedTo[LinkIndex];
				UScriptStruct* LinkType = Cast<UScriptStruct>(Link->PinType.PinSubCategoryObject.Get());

				if (RowStructType && RowStructType->IsChildOf(LinkType))
				{
					RowStructType = LinkType;
				}
			}
		}
	}
	
	return RowStructType;
}

void UK2Node_GetDataTableRowFromType::SetReturnTypeForStruct(UScriptStruct* NewRowStruct) const
{
	const UScriptStruct* OldRowStruct = GetReturnTypeForStruct();
	if (NewRowStruct != OldRowStruct)
	{
		UEdGraphPin* ResultPin = GetResultPin();

		if (ResultPin->SubPins.Num() > 0)
		{
			GetSchema()->RecombinePin(ResultPin);
		}

		// NOTE: purposefully not disconnecting the ResultPin (even though it changed type)... we want the user to see the old
		//       connections, and incompatible connections will produce an error (plus, some super-struct connections may still be valid)
		ResultPin->PinType.PinSubCategoryObject = NewRowStruct;
		ResultPin->PinType.PinCategory = (NewRowStruct == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;
	}
}

bool UK2Node_GetDataTableRowFromType::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetResultPin() && MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		bool bDisallowed = true;
		if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
		{
			if (const UScriptStruct* ConnectionType = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get()))
			{
				bDisallowed = !FDataTableEditorUtils::IsValidTableStruct(ConnectionType);
			}
		}
		else if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
		{
			bDisallowed = false;
		}

		if (bDisallowed)
		{
			OutReason = TEXT("Must be a struct that can be used in a DataTable");
		}
		return bDisallowed;
	}
	return false;
}

void UK2Node_GetDataTableRowFromType::PinDefaultValueChanged(UEdGraphPin* ChangedPin) 
{
	if (ChangedPin && ChangedPin->PinName == GetDataTableRowHelper::DataTypeValuePinName)
	{
		RefreshOutputPinType();
	}
}

void UK2Node_GetDataTableRowFromType::PostReconstructNode()
{
	Super::PostReconstructNode();

	RefreshOutputPinType();
}

void UK2Node_GetDataTableRowFromType::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);
    
    UEdGraphPin* OriginalDataTypeValueInPin = GetDataTypeValuePin();
	FDataTypeBase DataTypeValue;
	FDataTypeBase::StaticStruct()->ImportText(*OriginalDataTypeValueInPin->GetDefaultAsString(), &DataTypeValue, nullptr, EPropertyPortFlags::PPF_None, GLog, FDataTypeBase::StaticStruct()->GetName());
	if (!DataTypeValue.IsValid())
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("GetDataTableRowFromType_Error", "GetDataTableRowFromType must have a valid data type value specified!").ToString(), this);
		BreakAllNodeLinks();
		return;
	}

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UTypeToolboxLibrary, K2_GetDataTableRowFromValue);
	UK2Node_CallFunction* GetDataTableRowFromValueFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	GetDataTableRowFromValueFunction->FunctionReference.SetExternalMember(FunctionName, UTypeToolboxLibrary::StaticClass());
	GetDataTableRowFromValueFunction->AllocateDefaultPins();
    CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(GetDataTableRowFromValueFunction->GetExecPin()));

	// Connect the input of our GetDataTableRowFromValue to the Input of our Function pin
    UEdGraphPin* DataTypeValueInPin = GetDataTableRowFromValueFunction->FindPinChecked(TEXT("InTypeValue"));
	if(OriginalDataTypeValueInPin->LinkedTo.Num() > 0)
	{
		// Copy the connection
		CompilerContext.MovePinLinksToIntermediate(*OriginalDataTypeValueInPin, *DataTypeValueInPin);
	}
	else
	{
		// Copy literal
		DataTypeValueInPin->DefaultValue = OriginalDataTypeValueInPin->DefaultValue;
	}
	
	// Get some pins to work with
	UEdGraphPin* OriginalOutRowPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	UEdGraphPin* FunctionOutRowPin = GetDataTableRowFromValueFunction->FindPinChecked(TEXT("OutRow"));
    UEdGraphPin* FunctionReturnPin = GetDataTableRowFromValueFunction->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* FunctionThenPin = GetDataTableRowFromValueFunction->GetThenPin();
        
    // Set the type of the OutRow pin on this expanded mode to match original
    FunctionOutRowPin->PinType = OriginalOutRowPin->PinType;
	FunctionOutRowPin->PinType.PinSubCategoryObject = OriginalOutRowPin->PinType.PinSubCategoryObject;
        
    // BRANCH NODE
    UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
    BranchNode->AllocateDefaultPins();
	
    // Hook up inputs to branch
    FunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
    FunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());
        
    // Hook up outputs
    CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
    CompilerContext.MovePinLinksToIntermediate(*GetRowNotFoundPin(), *(BranchNode->GetElsePin()));
    CompilerContext.MovePinLinksToIntermediate(*OriginalOutRowPin, *FunctionOutRowPin);
	BreakAllNodeLinks();
}

void UK2Node_GetDataTableRowFromType::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);

	const UEdGraphPin* DataTypeValuePin = GetDataTypeValuePin();
	if (!DataTypeValuePin)
	{
		MessageLog.Error(*LOCTEXT("MissingPins", "Missing pins in @@").ToString(), this);
		return;
	}

	FDataTypeBase DataTypeValue;
	FDataTypeBase::StaticStruct()->ImportText(*DataTypeValuePin->GetDefaultAsString(), &DataTypeValue, nullptr, EPropertyPortFlags::PPF_None, GLog, FDataTypeBase::StaticStruct()->GetName());
	if (!DataTypeValue.IsValid())
	{
		MessageLog.Error(*LOCTEXT("InvalidDataType", "Data type value is invalid in @@").ToString(), this);
	}
}

void UK2Node_GetDataTableRowFromType::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetResultPin())
	{
		const UEdGraphPin* DataTypeValuePin = GetDataTypeValuePin();
		FDataTypeBase DataTypeValue;
		FDataTypeBase::StaticStruct()->ImportText(*DataTypeValuePin->GetDefaultAsString(), &DataTypeValue, nullptr, EPropertyPortFlags::PPF_None, GLog, FDataTypeBase::StaticStruct()->GetName());
		
		// This connection would only change the output type if the data type pin is invalid or undefined.
		const bool bIsTypeAuthority = (DataTypeValuePin->LinkedTo.Num() > 0 || !DataTypeValue.IsValid());
		if (bIsTypeAuthority)
		{
			RefreshOutputPinType();
		}		
	}
	else if (Pin == GetDataTypeValuePin())
	{
		const bool bConnectionAdded = Pin->LinkedTo.Num() > 0;
		if (bConnectionAdded)
		{
			// if the output connection was previously, incompatible, it now becomes the authority on this node's output type
			RefreshOutputPinType();
		}
	}
}

UScriptStruct* UK2Node_GetDataTableRowFromType::GetReturnTypeForStruct() const
{
	UScriptStruct* ReturnStructType = (UScriptStruct*)(GetResultPin()->PinType.PinSubCategoryObject.Get());
	return ReturnStructType;
}

UEdGraphPin* UK2Node_GetDataTableRowFromType::GetThenPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_GetDataTableRowFromType::GetDataTypeValuePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::DataTypeValuePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetDataTableRowFromType::GetRowNotFoundPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::RowNotFoundPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_GetDataTableRowFromType::GetResultPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

void UK2Node_GetDataTableRowFromType::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	const UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_GetDataTableRowFromType::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}

#undef LOCTEXT_NAMESPACE