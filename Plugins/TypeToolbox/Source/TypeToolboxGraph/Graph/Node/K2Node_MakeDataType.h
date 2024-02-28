// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_MakeDataType.generated.h"

class FBlueprintActionDatabaseRegistrar;
class FKismetCompilerContext;
class UEdGraphPin;

UCLASS(MinimalAPI, meta=(Keywords = "Data Type"))
class UK2Node_MakeDataType : public UK2Node
{
	GENERATED_UCLASS_BODY()
	
	//~ Begin UK2Node interface
	virtual bool IsNodePure() const override { return true; }
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual class FNodeHandlingFunctor* CreateNodeHandler(class FKismetCompilerContext& InCompilerContext) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	//~ End UK2Node interface

	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool ShouldShowNodeProperties() const override { return false; }
	//~ End UEdGraphNode Interface.

	UEdGraphPin* GetInputPin() const;
	UEdGraphPin* GetOutputPin() const;

private:
	
	/** Tooltip text for this node. */
	FText NodeTooltip;
};

