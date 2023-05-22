
#include "Damage/BaseStatusEffect.h"

#include "NiagaraComponent.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "NiagaraFunctionLibrary.h"

void ABaseStatusEffect::ApplyEffect(AElementCharacterBase* TargetCharacter)
{
	if(TargetCharacter!= nullptr)
    	{
    		if(StatusEffectVFX!= nullptr)
    		{
    			// Spawn niagara system attached to character mesh
    			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
    				StatusEffectVFX, 
    				TargetCharacter->GetRootComponent(),
    				VFXStartupCharacterSocket, 
    				FVector::ZeroVector,
    				FRotator::ZeroRotator,
    				EAttachLocation::KeepRelativeOffset,
    				true
    			);
    		}
    	}
}

void ABaseStatusEffect::RemoveEffect(AElementCharacterBase* TargetCharacter)
{
	if(NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
		NiagaraComponent->DestroyComponent();
	}
}
