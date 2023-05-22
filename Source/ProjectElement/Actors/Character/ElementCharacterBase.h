// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/AbilityComponent.h"
#include "Components/VitalityComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageInterface.h"
#include "ElementCharacterBase.generated.h"

class AWeaponBase;
class UDamageType;
class AElementBaseGameMode;

DECLARE_LOG_CATEGORY_EXTERN(LogElementCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElementCharacterDeathEvent, AElementCharacterBase*, InCharacterDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElementCharacterAbilityEvent, FGameplayTag, AbilityTag);

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class PROJECTELEMENT_API AElementCharacterBase : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	
	/** Health tracking component, used to mark a character as dead etc. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UVitalityComponent* HealthVitality = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAbilityComponent* AbilityComponent = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FElementCharacterDeathEvent OnCharacterDeathEvent;

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FElementCharacterAbilityEvent OnCharacterAbilityEventStarted;

	UPROPERTY(BlueprintAssignable, Category = "Character")
	FElementCharacterAbilityEvent OnCharacterAbilityEventEnded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsUsingUpperBodySlot = false;

protected:

	/** Pointer to characters capsule collision. */
	UPROPERTY(Transient, BlueprintReadWrite)
	UCapsuleComponent* CharacterCapsule = nullptr;

	/** Pointer to characters visual skeletal mesh. */
	UPROPERTY(Transient, BlueprintReadWrite)
	USkeletalMeshComponent* CharacterMesh = nullptr;
	
	/** Pointer to the game mode. */
	UPROPERTY(Transient)
	AElementBaseGameMode* GameMode = nullptr;

	bool bDead = false;
	
public:

	/** Constructor. */
	AElementCharacterBase();
	
	virtual void PostInitializeComponents() override;
	
	/** Level start. */
	virtual void BeginPlay() override;

	/** Called when the actor falls bellow the KillZ height,
	 *  overriden to place player back on nav mesh or at a player start. */
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	/** Simple function as an alternative to the nav mesh restarting at closest point.
	 *  Gets the player start actors and picks one at random. */
	virtual bool RestartPlayerRuntime();

	// ~ Begin IDamageInterface
	virtual bool Damage_Implementation(const UBaseDamageType* DamageType, AActor* InDamageSource) override;
	// ~ End IDamageInterface

	UFUNCTION(BlueprintCallable, Category = "Character")
	bool IsDead() const { return bDead; }
	
	/** Called to kill this player. */
	UFUNCTION(Exec, Category = "Character")
	void KillCharacter();
	
	/** Damage a given character from a given source. Returns true if the damage event depleted all health or vitality. */
	UFUNCTION(BlueprintCallable, Category = "Damage", meta = (DisplayName = "Damage Character"))
	bool K2_DamageCharacter(const UBaseDamageType* DamageType, AActor* InDamageSource);
	
	/** Called when health vitality runs out. */
	UFUNCTION()
	void OnDeathInternal();
	
	/** Called when health vitality re-gens. */
	UFUNCTION()
	void OnHealthRegenInternal(float InNewVitality, float InNewVitalityAlpha);

	/** Called when ability notify is triggered */
	UFUNCTION()
	void OnAbilityModuleNotify(FGameplayTag NotifyTag) const;

protected:

	virtual void KillCharacterInternal();
	virtual bool DamageCharacter(const UBaseDamageType* DamageType, AActor* InDamageSource); // Returns true if the damage event depleted all health or vitality.
	virtual void OnDeath();
	virtual void OnHealthRegen(float InNewVitality, float InNewVitalityAlpha) {};// Implement in base class. (UI)
};