
#include "Damage/HealStatusEffect.h"

#include "BaseDamageType.h"
#include "Actors/Character/ElementCharacterBase.h"

void AHealStatusEffect::ApplyEffect(AElementCharacterBase* TargetCharacter)
{
	Super::ApplyEffect(TargetCharacter);
	
	if (TargetCharacter)
	{
		CurrentTarget = TargetCharacter;
		
		GetWorld()->GetTimerManager().SetTimer(EffectTickHandle, this, &AHealStatusEffect::ApplyHeal, DamageTickInterval, true);
		
		GetWorld()->GetTimerManager().SetTimer(
			EffectDurationHandle,
			FTimerDelegate::CreateUObject(this, &AHealStatusEffect::RemoveEffect, TargetCharacter),
			HealDuration,
			false
			);
	}
}

void AHealStatusEffect::RemoveEffect(AElementCharacterBase* TargetCharacter)
{
	Super::RemoveEffect(TargetCharacter);

	if (TargetCharacter)
	{
		GetWorld()->GetTimerManager().ClearTimer(EffectDurationHandle);
		GetWorld()->GetTimerManager().ClearTimer(EffectTickHandle);
	}
}

void AHealStatusEffect::ApplyHeal()
{
	if(!EffectCauser)
	{
		return;
	}
	
	const AElementCharacterBase* TargetCharacter = Cast<AElementCharacterBase>(CurrentTarget);
	if (!TargetCharacter)
	{
		return;
	}

	if(UVitalityComponent* DamagedCharacterVitality = Cast<UVitalityComponent>(TargetCharacter->HealthVitality); DamagedCharacterVitality != nullptr)
	{
		UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>();		
		if(DamageTypeInstance != nullptr)
		{
			// invert so it heals
			// TODO :: Overriding BP instance? AGAIN? 
			DamageTypeInstance->DamageAmount = HealAmount * -1;
			DamageTypeInstance->HitReactionInfo = EHitReactionType::None;
			DamageTypeInstance->StatusEffectClass = Cast<UClass>(this->StaticClass());
			DamagedCharacterVitality->ApplyDamage(EffectCauser, DamageTypeInstance);
		}
	}
}
