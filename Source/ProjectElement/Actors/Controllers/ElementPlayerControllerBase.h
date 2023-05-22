// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "ElementPlayerControllerBase.generated.h"

class AElementCharacterBase;
class UBaseAbility;
class UAbilityComponent;
class AElementCharacter;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTELEMENT_API AElementPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:

#pragma region InputVariables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	float LookSensitivity = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	float ScrollSensitivity = 20.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	bool MouseInverted = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	float MinPitch = -80.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	float MaxPitch = -20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	float MinZoom = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Camera Boom")
	float MaxZoom = 1800.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultInputMappingContext = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LeftClickAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* RightClickAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* AbilityAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ScrollAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* ShiftAction = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D CurrentMouseInput;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D CurrentMovementInput;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bRightMouseHeld = false;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bLeftMouseHeld = false;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bShiftHeld = false;

#pragma endregion

	/** Constructor. */
	AElementPlayerControllerBase();

	/** Level start. */
	virtual void BeginPlay() override;

#if !UE_BUILD_SHIPPING

	/** Is god mode enabled? This player cannot be damaged. */
	bool bGodModeEnabled = false;

#endif
	
	/** Console command for killing all characters. */
	UFUNCTION(Exec, Category = "Characters")
	void KillAll() const;

	/** Console command for turning on god mode. */
	UFUNCTION(Exec)
	void ToggleGodMode();

	/** Setup menu input for interacting with the HUD. */
	virtual void SetupInputComponent() override;

#pragma region InputFunctions

	void Look(const FInputActionValue& ActionValue);
	void Move(const FInputActionValue& ActionValue);
	void ActivateAbility(const FInputActionValue& ActionValue);
	
	void OnLeftMouse(const FInputActionValue& ActionValue);
	void OnRightMouse(const FInputActionValue& ActionValue);

	void OnScroll(const FInputActionValue& ActionValue);
	void OnShift(const FInputActionValue& ActionValue);

#pragma endregion

	UPROPERTY(EditDefaultsOnly)
	UAbilityComponent* AbilityComponent = nullptr;

	UPROPERTY()
	TArray<FKey> AbilityKeys;

	UPROPERTY()
	UBaseAbility* CurrentAbility = nullptr;

	int32 CurrentAbilityIndex = 0;

	void ActivateCurrentAbility() const;

	void PrintCurrentAbility() const;
	

protected:

	UPROPERTY(Transient)
	AElementCharacter* ControlledElementCharacter = nullptr;

	UPROPERTY(Transient)
	AElementCharacterBase* ControlledElementCharacterBase = nullptr;
	
};
