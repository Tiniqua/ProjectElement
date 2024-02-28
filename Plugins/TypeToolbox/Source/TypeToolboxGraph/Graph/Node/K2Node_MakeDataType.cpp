// Fill out your copyright notice in the Description page of Project Settings.

#include "K2Node_MakeDataType.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFieldNodeSpawner.h"
#include "KismetCompilerMisc.h"
#include "EditorCategoryUtils.h"
#include "KismetCompiledFunctionContext.h"
#include "KismetCompiler.h"
#include "TypeToolbox/DataType/BaseDataType.h"

class FKCHandler_MakeDataType : public FNodeHandlingFunctor
{
	
public:
	
	FKCHandler_MakeDataType(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext) {}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
 		UK2Node_MakeDataType* MakeDataTypeNode = CastChecked<UK2Node_MakeDataType>(Node);
 		UEdGraphPin* OutputPin = MakeDataTypeNode->GetOutputPin();
		FNodeHandlingFunctor::RegisterNets(Context, Node);

		// Create a local term to drop the data type struct value into
		FBPTerminal* Term = Context.CreateLocalTerminalFromPinAutoChooseScope(OutputPin, Context.NetNameMap->MakeValidName(OutputPin));
		Term->bPassedByReference = false;
		Term->Source = Node;
 		Context.NetMap.Add(OutputPin, Term);
	}

	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		UK2Node_MakeDataType* MakeDataTypeNode = CastChecked<UK2Node_MakeDataType>(Node);
		UEdGraphPin* OutputPin = MakeDataTypeNode->GetOutputPin();
		FBPTerminal** OutputTerm = Context.NetMap.Find(OutputPin);
		check(OutputTerm);

		UEdGraphPin* InputPin = MakeDataTypeNode->GetInputPin();
		FBPTerminal** InputTerm = Context.NetMap.Find(InputPin);
		check(InputTerm);
		
		FBlueprintCompiledStatement& CreateDataTypeStatement = Context.AppendStatementForNode(Node);
		CreateDataTypeStatement.Type = KCST_Assignment;
		CreateDataTypeStatement.LHS = *OutputTerm;
		CreateDataTypeStatement.RHS.Add(*InputTerm);
	}
};

#define LOCTEXT_NAMESPACE "K2Node_GetDataTableRow"

UK2Node_MakeDataType::UK2Node_MakeDataType(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

FNodeHandlingFunctor* UK2Node_MakeDataType::CreateNodeHandler(FKismetCompilerContext& InCompilerContext) const
{
	return new FKCHandler_MakeDataType(InCompilerContext);
}

void UK2Node_MakeDataType::AllocateDefaultPins()
{
	UEdGraphPin* InputPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, TEXT("Data Type"));
	InputPin->PinType.PinSubCategoryObject = FDataTypeBase::StaticStruct();
	InputPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
	
	UEdGraphPin* OutputPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Struct, TEXT("Result"));
	OutputPin->PinType.PinSubCategoryObject = FDataTypeBase::StaticStruct();
	OutputPin->PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;

	Super::AllocateDefaultPins();
}

FText UK2Node_MakeDataType::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Make Data Type");
}

FText UK2Node_MakeDataType::GetTooltipText() const
{
	return NodeTooltip;
}

FSlateIcon UK2Node_MakeDataType::GetIconAndTint(FLinearColor& OutColor) const
{
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "GraphEditor.MakeStruct_16x");
	return Icon;
}

FLinearColor UK2Node_MakeDataType::GetNodeTitleColor() const
{
	if(const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>())
	{
		FEdGraphPinType PinType;
		PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		PinType.PinSubCategoryObject = FDataTypeBase::StaticStruct();
		return K2Schema->GetPinTypeColor(PinType);
	}
	return UK2Node::GetNodeTitleColor();
}

UEdGraphPin* UK2Node_MakeDataType::GetInputPin() const
{
	UEdGraphPin* Result = FindPin(TEXT("Data Type"), EGPD_Input);
	check(Result);
	return Result;
}

UEdGraphPin* UK2Node_MakeDataType::GetOutputPin() const
{
	UEdGraphPin* Result = FindPin(TEXT("Result"), EGPD_Output);
	check(Result);
	return Result;
}

void UK2Node_MakeDataType::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_MakeDataType::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}

bool UK2Node_MakeDataType::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if (MyPin == GetInputPin())
		{
			return true;
		}
		
		if (MyPin == GetOutputPin())
		{
			if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct && OtherPin->PinType.PinSubCategoryObject == FDataTypeBase::StaticStruct())
			{
				return false;
			}
		}
	}
	
	return true;
}

void UK2Node_MakeDataType::EarlyValidation(FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);
}

#undef LOCTEXT_NAMESPACE
