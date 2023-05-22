// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementPlayerControllerBase.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Character/ElementCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Camera/ElementCameraBoomArm.h"

AElementPlayerControllerBase::AElementPlayerControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//...
}

void AElementPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to character.
	ControlledElementCharacter = Cast<AElementCharacter>(GetCharacter());
	if (!ControlledElementCharacter)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: A subclass of AElementCharacter should be used with this controller. Things will not function correctly!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	
	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		AbilityKeys = Subsystem->QueryKeysMappedToAction(AbilityAction);
	}

	ControlledElementCharacterBase = Cast<AElementCharacterBase>(GetCharacter());
	if(ControlledElementCharacterBase)
	{
		AbilityComponent = ControlledElementCharacterBase->AbilityComponent;
	}
}

void AElementPlayerControllerBase::KillAll() const 
{
	const UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: No valid world?! Cannot run event!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	for (TActorIterator<AElementCharacterBase> It(CurrentWorld, AElementCharacterBase::StaticClass()); It; ++It)
	{
		AElementCharacterBase* FoundCharacter = *It;
		if (!FoundCharacter || FoundCharacter == GetCharacter())
		{
			continue;
		}

		FoundCharacter->KillCharacter();
	}
}

void AElementPlayerControllerBase::ToggleGodMode()
{
#if UE_BUILD_SHIPPING
	return;
#endif
	
	bGodModeEnabled = !bGodModeEnabled;
}

void AElementPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: UEnhancedInputComponent cannot be found please enable the EnhancedInput plugin!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::OnLeftMouse);
	EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::OnRightMouse);
	EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::OnScroll);
	EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::OnShift);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::Move);
	EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Triggered, this, &AElementPlayerControllerBase::ActivateAbility);
}

void AElementPlayerControllerBase::Look(const FInputActionValue& ActionValue)
{
	CurrentMouseInput = FVector2D(ActionValue[0], ActionValue[1]);

	if (!bRightMouseHeld)
	{
		return;
	}
	
	if (!ControlledElementCharacter)
	{
		return;
	}
	
	const UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		return;
	}
	
	const float PitchInput = (MouseInverted ? -1.0f : 1.0f) * (CurrentMouseInput.X * -1) * CurrentWorld->GetDeltaSeconds();
	const float YawInput = (MouseInverted ? -1.0f : 1.0f) * (CurrentMouseInput.Y * -1) * CurrentWorld->GetDeltaSeconds();
	ControlledElementCharacter->CameraBoom->AddRelativeRotation(FRotator(PitchInput, YawInput, 0.0f) * LookSensitivity);

	// Clamp the pitch.
	const FRotator Curr = ControlledElementCharacter->CameraBoom->GetRelativeRotation();
	const FRotator NewRelativeRotation = FRotator(FMath::Clamp(Curr.Pitch, MinPitch, MaxPitch), Curr.Yaw, 0.0f);
	ControlledElementCharacter->CameraBoom->SetRelativeRotation(NewRelativeRotation);
}

void AElementPlayerControllerBase::Move(const FInputActionValue& ActionValue)
{
	CurrentMovementInput = FVector2D(ActionValue[0], ActionValue[1]);
	
	if (!ControlledElementCharacter)
	{
		return;
	}

	FVector CameraForward = PlayerCameraManager->GetTransformComponent()->GetForwardVector();
	CameraForward.Z = 0.0f;
	CameraForward.Normalize();

	FVector CameraRight = PlayerCameraManager->GetTransformComponent()->GetRightVector();
	CameraRight.Z = 0.0f;
	CameraRight.Normalize();

	ControlledElementCharacter->AddMovementInput(CameraForward, CurrentMovementInput.Y);
	ControlledElementCharacter->AddMovementInput(CameraRight, CurrentMovementInput.X);
}

void AElementPlayerControllerBase::ActivateAbility(const FInputActionValue& ActionValue)
{
	if(CurrentAbility != nullptr)
	{
	 	CurrentAbility->EndAbility();
	}
	
	if (const UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		AbilityKeys = Subsystem->QueryKeysMappedToAction(AbilityAction);
	}
	
	if (AbilityComponent == nullptr || AbilityComponent->InstancedAbilities.Num() == 0)
	{
		return;
	}
	
	for (const FKey& Key : AbilityKeys)
	{
		if (IsInputKeyDown(Key))
		{
			// The current key is pressed, find its index in the AbilityKeys array
			const int32 Index = AbilityKeys.Find(Key);

			if (AbilityComponent->InstancedAbilities.IsValidIndex(Index))
			{
				// The index is valid, activate the corresponding ability
				CurrentAbility = AbilityComponent->InstancedAbilities[Index];
				PrintCurrentAbility();
				
				// Break out of the loop to prevent multiple activations
				break;
			}
		}
	}
}

void AElementPlayerControllerBase::OnLeftMouse(const FInputActionValue& ActionValue)
{
	const bool bPressed = (bool)ActionValue[0];
	bLeftMouseHeld = bPressed;
	if (bPressed)
	{
		// Left mouse button is held down
		// TODO - Check if we can activate ability (will need cooldowns maybe)
		if (CurrentAbility != nullptr)
		{
			ActivateCurrentAbility();
		}
	}
}

void AElementPlayerControllerBase::OnRightMouse(const FInputActionValue& ActionValue)
{
	const bool bPressed = (bool)ActionValue[0];
	bRightMouseHeld = bPressed;
	if (bPressed)
	{

		return;
	}
	
	if (CurrentAbility != nullptr)
	{
		CurrentAbility->EndAbility();
	}
}

void AElementPlayerControllerBase::OnScroll(const FInputActionValue& ActionValue)
{
	const float ScrollInput = ActionValue[0];
	if (!ControlledElementCharacter)
	{
		return;
	}

	if(!bShiftHeld)
	{
		const float CurrentCameraBoomLength = ControlledElementCharacter->CameraBoom->TargetArmLength;
		const float NewCameraBoomLength = FMath::Clamp(CurrentCameraBoomLength + (ScrollInput * ScrollSensitivity * -1.0f), MinZoom, MaxZoom);
		ControlledElementCharacter->CameraBoom->TargetArmLength = NewCameraBoomLength;
	}	
}

void AElementPlayerControllerBase::OnShift(const FInputActionValue& ActionValue)
{
	const bool bPressed = (bool)ActionValue[0];
	bShiftHeld = bPressed;
	if (bPressed)
	{

		return;
	}
}

void AElementPlayerControllerBase::ActivateCurrentAbility() const
{
	if (CurrentAbility == nullptr)
	{
		return;
	}

	if (CurrentAbility->IsTelegraphedAbility && CurrentAbility->IsTelegraphPendingConfirm)
	{
		CurrentAbility->ActivateTelegraphedAbility();
	}
	else
	{
		CurrentAbility->StartAbility();
	}
}

void AElementPlayerControllerBase::PrintCurrentAbility() const
{
	if (CurrentAbility == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No current ability selected"));
	}
	else
	{
		FString AbilityName = CurrentAbility->GetName();
		FVector Location = ControlledElementCharacter->GetActorLocation();

		// Offset the location based on the ability index to avoid overlapping text
		float OffsetZ = 50.f * CurrentAbilityIndex;
		FVector Offset(0.f, 0.f, OffsetZ);
		Location += Offset;
		
		DrawDebugString(GetWorld(), Location, FString::Printf(TEXT("Current ability: %s"), *AbilityName), nullptr, FColor::Yellow, 1.0f, false);
	}
}
