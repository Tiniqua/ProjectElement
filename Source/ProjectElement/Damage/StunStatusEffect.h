
#pragma once

#include "CoreMinimal.h"
#include "Damage/BaseStatusEffect.h"
#include "StunStatusEffect.generated.h"

UCLASS(Blueprintable, BlueprintType)
class PROJECTELEMENT_API AStunStatusEffect : public ABaseStatusEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stun")
	float StunDuration;

	virtual void ApplyEffect(AElementCharacterBase* TargetCharacter) override;
	virtual void RemoveEffect(AElementCharacterBase* TargetCharacter) override;
};
