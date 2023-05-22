
#include "Animation/AbilityNotify.h"

#include "Actors/Character/ElementCharacterBase.h"

void UAbilityNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(const AElementCharacterBase* Character = Cast<AElementCharacterBase>(MeshComp->GetOwner()))
	{
		Character->OnAbilityModuleNotify(NotifyTag);
	}
}
