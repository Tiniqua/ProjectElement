// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DataTableEditorUtils.h"
#include "StructViewerFilter.h"

class UScriptStruct;

/** Editor utilities for getting custom slate code and filters for Type Toolbox Variables. */
class FTypeToolboxUtils
{
	
public:

	/** Returns if the given static structure is a valid default type base. (FDataTypeBase::StaticStruct) */
	static bool IsValidDataTypeBaseStruct(const UScriptStruct* Struct);

	/** Returns list of all structures that are children of the FDataTypeBase::StaticStruct() */
	static TArray<UScriptStruct*> GetAllDataTypeBaseStruct();
};

class FStructViewerInitializationOptions;
class FStructViewerFilterFuncs;

class FDataTableStructFilter : public IStructViewerFilter
{
public:
	
	virtual bool IsStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const UScriptStruct* InStruct, TSharedRef<FStructViewerFilterFuncs> InFilterFuncs) override
	{
		return FDataTableEditorUtils::IsValidTableStruct(InStruct);
	}

	virtual bool IsUnloadedStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const FSoftObjectPath& InStructPath, TSharedRef<FStructViewerFilterFuncs> InFilterFuncs) override
	{
		// Unloaded structs are always User Defined Structs, and User Defined Structs are always allowed for table row types.
		return true;
	}
};

class FDataTypeBaseStructFilter : public IStructViewerFilter
{
public:
	
	virtual bool IsStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const UScriptStruct* InStruct, TSharedRef<FStructViewerFilterFuncs> InFilterFuncs) override
	{
		return FTypeToolboxUtils::IsValidDataTypeBaseStruct(InStruct);
	}

	virtual bool IsUnloadedStructAllowed(const FStructViewerInitializationOptions& InInitOptions, const FSoftObjectPath& InStructPath, TSharedRef<FStructViewerFilterFuncs> InFilterFuncs) override
	{
		return false;
	}
};