// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_GetDataTableRowFromType.generated.h"

UCLASS(MinimalAPI, meta=(Keywords = "Get Data Table Row"))
class UK2Node_GetDataTableRowFromType : public UK2Node
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PostReconstructNode() override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual FText GetMenuCategory() const override;
	//~ End UK2Node Interface

	/** Get the then output pin */
	UEdGraphPin* GetThenPin() const;
	
	/** Get the Data Table input pin */
	UEdGraphPin* GetDataTypeValuePin() const;
	
	/** Get the exec output pin for when the row was not found */
	UEdGraphPin* GetRowNotFoundPin() const;
	
	/** Get the result output pin */
	UEdGraphPin* GetResultPin() const;
	
protected:

	/** Queries for the authoritative return type, then modifies the return pin to match */
	void RefreshOutputPinType() const;

	/** Get the type of the TableRow to return */
	UScriptStruct* GetDataTableRowStructType() const;

	/** Set the return type of our struct */
	void SetReturnTypeForStruct(UScriptStruct* NewRowStruct) const;

	/** Get the return type of our struct */
	UScriptStruct* GetReturnTypeForStruct() const;
};