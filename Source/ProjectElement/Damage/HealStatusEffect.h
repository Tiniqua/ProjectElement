

#pragma once

#include "CoreMinimal.h"
#include "Damage/BaseStatusEffect.h"
#include "HealStatusEffect.generated.h"

UCLASS()
class PROJECTELEMENT_API AHealStatusEffect : public ABaseStatusEffect
{
	GENERATED_BODY()

	virtual void ApplyEffect(AElementCharacterBase* TargetCharacter) override;
	virtual void RemoveEffect(AElementCharacterBase* TargetCharacter) override;

	void ApplyHeal();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	float HealDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealAmount = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float DamageTickInterval = 1.f;

private:
	UPROPERTY()
	AActor* CurrentTarget = nullptr;
};
