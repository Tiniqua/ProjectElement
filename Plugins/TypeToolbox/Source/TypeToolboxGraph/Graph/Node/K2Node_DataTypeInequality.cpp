// Fill out your copyright notice in the Description page of Project Settings.

#include "K2Node_DataTypeInequality.h"
#include "TypeToolbox/DataType/TypeToolboxLibrary.h"

#define LOCTEXT_NAMESPACE "UK2Node_DataTypeInequality"

UK2Node_DataTypeInequality::UK2Node_DataTypeInequality(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

FText UK2Node_DataTypeInequality::GetTooltipText() const
{
	return LOCTEXT("DataTypeInequalityTooltip", "Returns true if A is NOT equal to B (A != B)");
}

FText UK2Node_DataTypeInequality::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("K2Node_DataTypeInequality", "NotEqualDataType", "Not Equal (DataType)");
}

FName UK2Node_DataTypeInequality::GetConditionalFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(UTypeToolboxLibrary, NotEqual_DataTypeDataType);
}

#undef LOCTEXT_NAMESPACE