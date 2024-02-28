// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EdGraphUtilities.h"

class SGraphPin;
class UEdGraphPin;

class FTypeToolboxPinFactory : public FGraphPanelPinFactory
{

public:
	
	/** Pin creation factory based on custom pin implementations. E.g SListDataTypePin... */
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override;
};