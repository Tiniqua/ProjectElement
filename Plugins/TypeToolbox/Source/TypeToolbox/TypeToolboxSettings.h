// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TypeToolboxSettings.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogToolboxSettings, Log, All);

/**
 * Toolbox user settings (Project settings)
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName = "Type Toolbox"))
class TYPETOOLBOX_API UTypeToolboxSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	/** List of the active data type definitions.
	 *  NOTE: Mapping of the FDataTypeBase sub-struct to FDataTypeSettings settings.
	 *  Key: Type of FDataTypeBaseId this type is. (e.g FDataTypeBase etc.)
	 *  Value: FDataTypeSettings, the type of row to use for this type and a list of active data tables for this type. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TMap<FDataTypeSelector, FDataTypeSettings> DataTypes;
	
	UTypeToolboxSettings() {}

#if WITH_EDITOR
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
		if (!DataTypeSubsystem)
		{
			UE_LOG(LogToolboxSettings, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
		
		DataTypeSubsystem->Editor_MarkNeedsRefresh();
	}
	
#endif
};
