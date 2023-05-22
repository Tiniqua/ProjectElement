
#include "Damage/BaseDamageType.h"

void UBaseDamageType::ApplyStatusEffect(AElementCharacterBase* TargetCharacter)
{
	if (StatusEffectClass && TargetCharacter)
	{
		ABaseStatusEffect* StatusEffect = NewObject<ABaseStatusEffect>(this, StatusEffectClass);
		StatusEffect->ApplyEffect(TargetCharacter);
	}
}
