// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VitalityComponent.generated.h"

class UBaseDamageType;
DECLARE_LOG_CATEGORY_EXTERN(LogVitalityComponent, Warning, All);

class AElementCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVitalityDamageEvent, float, InDamage, AActor*, InDamageSource);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVitalityRegenEvent, float, InNewVitality, float, InNewVitalityAlpha);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVitalityEvent);

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Player,
	Enemy,
	World
};

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class PROJECTELEMENT_API UVitalityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality")
	FName VitalityName = NAME_None;

	/** Base amount of vitality. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality")
	float MaxAmount = 100.0f;

	/** Starting amount of vitality. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality")
	float StartAmount = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vitality")
	EFaction Faction;

	/** Multiplier for MaxAmount, this can be applied externally during runtime for buffs. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality")
	float VitalityMultiplierOverride = 1.0f;

	/** Will this vitality regen over time if it is not damaged for a given regen cooldown. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality")
	bool bRegenOverTime = true;

	/** If bRegenOverTime is enabled the vitality component will wait this amount of time before starting to regen. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality", meta = (EditCondition = "bRegenOverTime"))
	float RegenCooldown = 10.0f;

	/** If bRegenOverTime, How often to regen a given RegenAmount to the CurrentAmount vitality value. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality", meta = (EditCondition = "bRegenOverTime"))
	float RegenRate = 2.0f;

	/** If bRegenOverTime, How much to regen each time a regen event is ran. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Vitality", meta = (EditCondition = "bRegenOverTime"))
	float RegenAmount = 5.0f;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FVitalityDamageEvent OnReceivedDamageEvent;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FVitalityEvent OnVitalityDepletedEvent;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FVitalityEvent OnVitalityResetEvent;

	UPROPERTY(BlueprintAssignable, Category = "Vitality")
	FVitalityRegenEvent OnVitalityRegenEvent;

protected:

	/** Current vitality amount. */
	UPROPERTY(BlueprintReadOnly, Category = "Vitality")
	float CurrentAmount = 0.0f;

	FTimerHandle RegenTimerHandle;
	FTimerDelegate RegenDelegate;

private:

	bool bMarkedDepleted = false;
	
public:

	/** Constructor. */
	UVitalityComponent();

	/** Level start. */
	virtual void BeginPlay() override;

	/** Apply damage to the given vitality data. */
	UFUNCTION(BlueprintCallable, Category = "Vitality")
	bool ApplyDamage(AActor* InDamageSource, const UBaseDamageType* DamageType);

	/** Reset the vitality to the initial state on begin play. */
	void ResetVitality();

	/** If the current vitality is depleted or not at max vitality this can be used to force regen. */
	UFUNCTION()
	void StartRegen(bool bStartFromDepleted = false);

	/** Called on a timer to regen the current vitality over time. */
	UFUNCTION()
	void RegenEvent();

	/** Mark the vitality as depleted and stop all regen, this must be done manually. */
	void MarkVitalityDepleted();

	UFUNCTION(BlueprintCallable)
	float GetCurrentVitality() const { return CurrentAmount; }

	/** Returns the current vitality as a percentage between 0-1 mapped to 0-MaxVitality */
	UFUNCTION(BlueprintCallable)
	float GetCurrentVitalityAlpha() const;
};
