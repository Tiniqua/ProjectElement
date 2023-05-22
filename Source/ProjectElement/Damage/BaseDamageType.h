#pragma once

#include "CoreMinimal.h"
#include "BaseStatusEffect.h"
#include "GameFramework/DamageType.h"
#include "BaseDamageType.generated.h"

UENUM(BlueprintType)
enum class EHitReactionType : uint8
{
	None,
	Light,
	Standard,
	Heavy
};

// TODO - Support status effect to apply as part of damage, status effect can support VFX, Sound, Capability changes etc..
UCLASS()
class PROJECTELEMENT_API UBaseDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	TSubclassOf<ABaseStatusEffect> StatusEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Reaction")
	EHitReactionType HitReactionInfo = EHitReactionType::Light;

	void ApplyStatusEffect(AElementCharacterBase* TargetCharacter);
};
