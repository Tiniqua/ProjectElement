// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "K2Node_DataTypeEquality.h"
#include "K2Node_DataTypeInequality.generated.h"

UCLASS(MinimalAPI, meta=(Keywords = "!="))
class UK2Node_DataTypeInequality : public UK2Node_DataTypeEquality
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UEdGraphNode Interface

	//~ Begin UK2Node Interface
	virtual FText GetCompactNodeTitle() const override { return NSLOCTEXT("K2Node", "NotEqual", "!="); }
	//~ End UK2Node Interface

protected:

	/** Override from the equality function to check for != instead. (A != B). */
	virtual FName GetConditionalFunctionName() const override;
};