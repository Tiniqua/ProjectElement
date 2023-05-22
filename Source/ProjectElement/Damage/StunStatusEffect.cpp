
#include "Damage/StunStatusEffect.h"

#include "Actors/Character/ElementCharacter.h"

void AStunStatusEffect::ApplyEffect(AElementCharacterBase* TargetCharacter)
{
	Super::ApplyEffect(TargetCharacter);
	
	if (!TargetCharacter) return;
	
	if(AElementCharacter* PlayerCharacter = Cast<AElementCharacter>(TargetCharacter))
	{
		if(AElementPlayerControllerBase* PlayerControllerBase = Cast<AElementPlayerControllerBase>(PlayerCharacter->GetPlayerController()))
		{
			PlayerControllerBase->DisableInput(PlayerControllerBase);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(EffectDurationHandle, FTimerDelegate::CreateUObject(this, &AStunStatusEffect::RemoveEffect, TargetCharacter), StunDuration, false);
}

void AStunStatusEffect::RemoveEffect(AElementCharacterBase* TargetCharacter)
{
	Super::RemoveEffect(TargetCharacter);
	
	if (!TargetCharacter) return;

	if(AElementCharacter* PlayerCharacter = Cast<AElementCharacter>(TargetCharacter))
	{
		if(AElementPlayerControllerBase* PlayerControllerBase = Cast<AElementPlayerControllerBase>(PlayerCharacter->GetPlayerController()))
		{
			PlayerControllerBase->EnableInput(PlayerControllerBase);
		}
	}

	GetWorld()->GetTimerManager().ClearTimer(EffectDurationHandle);
}