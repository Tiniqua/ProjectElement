
#include "Damage/DOTStatusEffect.h"
#include "BaseDamageType.h"
#include "Actors/Character/ElementCharacterBase.h"

void ADOTStatusEffect::ApplyEffect(AElementCharacterBase* TargetCharacter)
{
	Super::ApplyEffect(TargetCharacter);

	if (TargetCharacter)
	{
		CurrentTarget = TargetCharacter;
		
		GetWorld()->GetTimerManager().SetTimer(EffectTickHandle, this, &ADOTStatusEffect::DoDamage, DamageTickInterval, true);
		
		GetWorld()->GetTimerManager().SetTimer(
			EffectDurationHandle,
			FTimerDelegate::CreateUObject(this, &ADOTStatusEffect::RemoveEffect, TargetCharacter),
			DOTDuration,
			false
			);
	}
}

void ADOTStatusEffect::RemoveEffect(AElementCharacterBase* TargetCharacter)
{
	Super::RemoveEffect(TargetCharacter);

	if (TargetCharacter)
	{
		GetWorld()->GetTimerManager().ClearTimer(EffectDurationHandle);
		GetWorld()->GetTimerManager().ClearTimer(EffectTickHandle);
	}
}

void ADOTStatusEffect::DoDamage() const
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
	
	UVitalityComponent* DamagedCharacterVitality = Cast<UVitalityComponent>(TargetCharacter->HealthVitality);
	if(DamagedCharacterVitality != nullptr)
	{
		UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>();		
		if(DamageTypeInstance != nullptr)
		{
			DamageTypeInstance->DamageAmount = DamageAmount;
			DamagedCharacterVitality->ApplyDamage(EffectCauser, DamageTypeInstance);
		}
	}
}
