// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Camera/ElementCameraBoomArm.h"
#include "Damage/BaseDamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Modes/ElementBaseGameMode.h"
#include "UI/ElementHUD.h"

AElementCharacter::AElementCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	CameraBoom = CreateDefaultSubobject<UElementCameraBoomArm>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = true;
}

void AElementCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AElementPlayerControllerBase>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: Character %s has no AElementPlayerControllerBase, it will not function correctly!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
	}

	ElementHUD = Cast<AElementHUD>(PlayerController->GetHUD());
	if (!ElementHUD)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: No valid AElementHUD, please use the correct hud class to toggle the in-game menu!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
}

void AElementCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//...
}

bool AElementCharacter::DamageCharacter(const UBaseDamageType* DamageType, AActor* InDamageSource)
{
#if !UE_BUILD_SHIPPING
	if (PlayerController && PlayerController->bGodModeEnabled)
	{
		return false;
	}
#endif
	
	// Dead...
	if (Super::DamageCharacter(DamageType, InDamageSource))
	{
		if (ElementHUD)
		{
			// ElementHUD->GetGameplayUI()->CurrentHealth = 0.0f;
			// ElementHUD->GetGameplayUI()->OnDeath();
		}
		return true;
	}

	if (ElementHUD)
	{
		// ElementHUD->GetGameplayUI()->CurrentHealth = HealthVitality->GetCurrentVitalityAlpha();
	}
	
	return false;
}