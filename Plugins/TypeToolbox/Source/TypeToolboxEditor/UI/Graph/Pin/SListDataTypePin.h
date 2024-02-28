// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SListStructTypePin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SListDataTypePin : public SListStructTypePin
{
	
public:
	
	SLATE_BEGIN_ARGS(SListDataTypePin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
	
	/** Return current set value as text derived from the FDataTypeSelector structure. */
	virtual FText GetComboTextValue() const override;

	/** Set the correct item set . */
	virtual void OnStructPicked(const UScriptStruct* PickedStruct) const override;
};