// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TypeToolboxLibrary.generated.h"

/**
 * Functions for easily accessing the data type subsystem and validating data types.
 */
UCLASS()
class TYPETOOLBOX_API UTypeToolboxLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/** Returns true if the data type is currently loaded into the DataTypeSubsystem successfully. False if not. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Is Data Type Valid"))
	static bool IsDataTypeValid(const FDataTypeBase InDataType);

	/** Returns the FDataTypeSettings as TOptional defined for this data base index location. Can use IsSet to check if it was found or not.
	 *  @In InIndex: The data table index you are searching for as these will match the data type settings index values. */
	static TOptional<FDataTypeSettings> GetDataTypeSettingsFromIndex(const int32 InIndex);
	
	/** Returns the FDataTypeSettings as TOptional defined for this structure type. Can use IsSet to check if it was found or not.
	 *  @In InScriptStruct: The structure type you are searching for settings for. */
	static TOptional<FDataTypeSettings> GetDataTypeSettingsFromTypeStruct(const UScriptStruct* InScriptStruct);

	/** Finds the FDataTypeSettings defined for this structure type. Returns true if found false if not.
	 *  NOTE: Returns useful information such as prefix name, UI data etc.
	 *  @In InType: The data type you are searching for settings for.
	 *  @Out OutFoundSettings: The found FDataTypeSettings for the given data type. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get Data Type Settings From Type"))
	static bool GetDataTypeSettingsFromType(FDataTypeSelector InType, FDataTypeSettings& OutFoundSettings);

	/** Finds the data table loaded for a given row name if it is listed as a valid data type. Returns true if found false if not.
 	 *  @In InRowName: The data type row name you want the data table from. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get Data Table From Type Row Name"))
	static UDataTable* GetDataTableFromTypeRowName(const FString& InRowName);
	
	/** Finds the data table loaded for a given type of data structure. Returns true if found false if not.
	  *  @In InScriptStruct: The data types structure (e.g. FDataTypeBase::StaticStruct). */
	static UDataTable* GetDataTableFromTypeStruct(const UScriptStruct* InScriptStruct);
	
	/** Finds the data table loaded for a given type of data. Returns true if found false if not.
	 *  @In InType: The data type you are searching for settings for. (e.g. FDataTypeBase)
	 *  @Out OutDataTable: The found data table for the given data type. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get Data Table From Type"))
	static bool GetDataTableFromType(FDataTypeSelector InType, UDataTable*& OutDataTable);
	
	/** Finds the data table loaded for a given data type value using its internal data table index. Returns true if found false if not.
	 *  @In InTypeValue: The data type value you are searching for. (e.g. set variable of type FDataTypeBase or child)
	 *  @Out OutDataTable: The found data table for the given data type value. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get Data Table From Value"))
	static bool GetDataTableFromValue(const FDataTypeBase InTypeValue, UDataTable*& OutDataTable);

	/** Returns the data table row type for a given data type value. Or null if it is invalid.
	 *  @In InTypeValue: The data type value you are searching for. (e.g. set variable of type FDataTypeBase or child) */
	static UScriptStruct* GetDataTypeRowType(const FDataTypeBase InTypeValue);
	
	/** Returns the data table row of type RowType_T loaded for a given data type value using its internal data table index and row name.
	 *  @In InTypeValue: The data type value you are searching for. (e.g. set variable of type FDataTypeBase or child) */
	template<typename RowType_T>
	static RowType_T* GetDataTableRowFromValue(const FDataTypeBase InTypeValue);

	/** Returns the data table rows of type RowType_T loaded for a given data type.
	 *  @In InScriptStruct: The data type struct you are searching for. (e.g. FDataTypeBase::StaticStruct())
	 *  Returns a TArray<RowType_T>(). */
	template<typename RowType_T>
	static TArray<RowType_T*> GetDataTableRowsFromTypeStruct(const UScriptStruct* InScriptStruct);
	
	/** Gets a list of all data type values that are loaded/valid within the data type subsystem.
	 *  Returns a Set<FDataTypeBase>(). */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get All Data Types"))
	static TSet<FDataTypeBase> GetAllDataTypeValues();

	/** Gets a list of all data type values for a given type that are loaded/valid within the data type subsystem.
	 *  @In InScriptStruct: The data type structure you are searching. (e.g. FDataTypeBase::StaticStruct())
	 *  Returns a Set<FDataTypeBase>(). */
	static TSet<FDataTypeBase> GetAllDataTypeValuesForStruct(const UScriptStruct* InScriptStruct);

	/** Gets a list of all data type values for a given type that are loaded/valid within the data type subsystem.
	 *  @In InType: The data type you are searching. (e.g. FDataTypeBase)
	 *  Returns a Set<FDataTypeBase>(). */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get All Data Type Values For Type"))
	static TSet<FDataTypeBase> GetAllDataTypeValuesForType(FDataTypeSelector InType);

	/** Returns bitmask status mask from data type subsystem for the given type value if it exists. 0 if not.
	 *  @In InTypeValue: The data type value you are searching for. (e.g. set variable of type FDataTypeBase or child) */
	static int32 GetStatusMaskForValue(const FDataTypeBase InTypeValue);

	/** Gets FTypeUserInterfaceInfo structure from the row definition of a given data type.
	 *  @In InType: The data type value you are searching for. (e.g. set variable of type FDataTypeBase or child) 
	 *  Returns a FTypeUserInterfaceInfo containing a UI display name and description. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get All Data Type Values For Type"))
	static FTypeUserInterfaceInfo GetUserInterfaceInfoFromValue(const FDataTypeBase InTypeValue);

	/** Gets a composite UDataTable* that contains all data tables for all types.
 	 *  @Out OutDataTable: The created composite data table for all data type values.
 	 *  Returns true if data types have been found to create UDataTable* from, false if there is no data types to do so. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get All Data Types As Data Table"))
	static bool GetAllDataTypesAsDataTable(UDataTable*& OutDataTable);

	/** Find data table index from the data table that is loaded into the DataTypeSubsystem.
 	 *  @In InDataTable: Data table searching for the index.
 	 *  Returns the found index of the given data table. Returns -1 if it cannot find the data table or the subsystem cannot be found. */
	UFUNCTION(BlueprintCallable, Category= "Data Type", meta = (DisplayName = "Get All Data Type Values For Type"))
	static int32 GetDataTableIndex(const UDataTable* InDataTable);

	/** Returns true if A is equal to B (A == B) in terms of data types. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (DataType)", CompactNodeTitle = "==", Keywords = "== equal"))
	static bool EqualEqual_DataTypeDataType(const FDataTypeBase A, const FDataTypeBase B);

	/** Returns true if A is not equal to B (A != B) in terms of data types. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Not Equal (DataType)", CompactNodeTitle = "!=", Keywords = "!= equal"))
	static bool NotEqual_DataTypeDataType(const FDataTypeBase A, const FDataTypeBase B);	

	// NOTE: This function is used for the K2Node_GetDataTableRowFromType
	/** Returns true if a table row was found for the given data type value. False if not.
	*  @In InTypeValue: The data type value you are searching for. (e.g. set variable of type FDataTypeBase or child)
	*  @Out OutRow: The found table row for the searched data type value. */
	UFUNCTION(BlueprintCallable, Category = "Data Type", meta=(CustomStructureParam = "OutRow", BlueprintInternalUseOnly="true"))
	static bool K2_GetDataTableRowFromValue(const FDataTypeBase InTypeValue, FTableRowBase& OutRow);
};