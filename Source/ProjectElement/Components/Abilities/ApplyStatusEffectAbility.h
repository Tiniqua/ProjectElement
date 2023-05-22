
#pragma once

#include "CoreMinimal.h"
#include "Components/VitalityComponent.h"
#include "Components/Abilities/BaseAbility.h"
#include "ApplyStatusEffectAbility.generated.h"

class ABaseStatusEffect;

UCLASS()
class PROJECTELEMENT_API UApplyStatusEffectAbility : public UBaseAbility
{
	GENERATED_BODY()

	virtual void StartAbility() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	TSubclassOf<ABaseStatusEffect> StatusEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	float RadiusToApply = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	bool DrawDebug = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	EFaction FactionToApplyTo = EFaction::Enemy;
};
