
#pragma once

#include "CoreMinimal.h"
#include "Damage/BaseStatusEffect.h"
#include "DOTStatusEffect.generated.h"

UCLASS()
class PROJECTELEMENT_API ADOTStatusEffect : public ABaseStatusEffect
{
	GENERATED_BODY()

	virtual void ApplyEffect(AElementCharacterBase* TargetCharacter) override;
	virtual void RemoveEffect(AElementCharacterBase* TargetCharacter) override;

	void DoDamage() const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float DOTDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageTickInterval = 0.5f;

private:
	UPROPERTY()
	AActor* CurrentTarget = nullptr;
};
