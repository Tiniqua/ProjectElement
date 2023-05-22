// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ElementCharacterBase.h"
#include "Actors/Controllers/ElementPlayerControllerBase.h"
#include "ElementCharacter.generated.h"

class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UElementCameraBoomArm;
class AElementHUD;

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class PROJECTELEMENT_API AElementCharacter : public AElementCharacterBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Camera")
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character|Camera")
	UElementCameraBoomArm* CameraBoom = nullptr;
	
	/** Mouse speed to effect the rate at which the user can turn and look around. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Movement")
	float MouseSpeed = 45.0f;

protected:
	
	UPROPERTY(Transient)
	AElementPlayerControllerBase* PlayerController = nullptr;

	UPROPERTY(Transient)
	AElementHUD* ElementHUD = nullptr;

public:

	/** Constructor. */
	AElementCharacter();
	
	/** Returns PlayerCamera. **/
	UCameraComponent* GetCamera() const { return Camera; }

	/** Returns this characters RSPlayerController class. */
	AElementPlayerControllerBase* GetPlayerController() { return PlayerController; }

protected:

	/** Level start. */
	virtual void BeginPlay() override;
	
	/** Frame. */
	virtual void Tick(float DeltaSeconds) override;
	
	/** Update health UI and if not shipping check god mode is not active. */
	virtual bool DamageCharacter(const UBaseDamageType* DamageType, AActor* InDamageSource) override;
};