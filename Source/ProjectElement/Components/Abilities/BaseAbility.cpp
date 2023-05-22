
#include "Components/Abilities/BaseAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Actors/Character/ElementCharacterBase.h"


UBaseAbility::UBaseAbility()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseAbility::SetupAbility()
{
	if(OwnerCharacter != nullptr)
	{
		// called by anim notifies
		AElementCharacterBase* Character = Cast<AElementCharacterBase>(OwnerCharacter);
		if(Character != nullptr)
		{
			Character->OnCharacterAbilityEventStarted.AddUniqueDynamic(this, &UBaseAbility::StartAbilityIfTagsMatch);
		}
	}
}

void UBaseAbility::PlayStartupMontage()
{
	if(IsPlayingAnimation)
	{
		return;
	}
	
	if(IsValid(StartupMontage))
	{
		if(UpperBodyAnim)
		{
			AElementCharacterBase* CharacterBase = Cast<AElementCharacterBase>(OwnerCharacter);
			CharacterBase->IsUsingUpperBodySlot = true;
		}
		
		if(OwnerCharacter!= nullptr)
		{
			const USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
			if(OwnerMesh != nullptr)
			{
				if(UAnimInstance* Instance = OwnerMesh->GetAnimInstance(); Instance != nullptr)
				{
					const float Duration = Instance->Montage_Play(StartupMontage,1,EMontagePlayReturnType::Duration);
					IsPlayingAnimation = true;
					if (Duration > 0.0f)
					{
						if(const UWorld* World = GetWorld())
						{
							World->GetTimerManager().SetTimer(AbilityMontage, this, &UBaseAbility::ResetAnimState, Duration, false);
						}
					}
				}
			}
		}
	}
}

void UBaseAbility::StartAbility()
{
	if (IsAbilityOnCooldown)
	{
		return;
	}
	
	IsAbilityOnCooldown = true;

	if (CooldownDuration > 0.0f)
	{
		FTimerHandle CooldownResetTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(CooldownResetTimerHandle, this, &UBaseAbility::ResetCooldown, CooldownDuration, false);
	}
	
	if(StartupMontage && !IsTelegraphedAbility)
	{
		PlayStartupMontage();
		return;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if(OwnerCharacter!= nullptr)
	{
		if(AbilityStartupVFX!= nullptr)
		{
			// Spawn niagara system attached to character mesh
			UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				AbilityStartupVFX, 
				OwnerCharacter->GetRootComponent(),
				VFXStartupCharacterSocket, 
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}
	}

	if(!StartupMontage)
	{
		EndAbility();
	}
}

void UBaseAbility::ResetCooldown()
{
	IsAbilityOnCooldown = false;
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
}

void UBaseAbility::ActivateTelegraphedAbility()
{
	IsTelegraphPendingConfirm = false;
}

void UBaseAbility::EndAbility()
{
	// Clear the cooldown timer handle to avoid memory leaks
	if (CooldownTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}

void UBaseAbility::StartAbilityIfTagsMatch(FGameplayTag TriggeredAbilityTag)
{
	if (AbilityTag == TriggeredAbilityTag)
	{
		StartAbility();
	}
}

void UBaseAbility::ResetAnimState()
{
	if(OwnerCharacter!= nullptr)
	{
		USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
		if(OwnerMesh != nullptr)
		{
			OwnerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			IsPlayingAnimation = false;
		}
	}

	if(UpperBodyAnim)
	{
		AElementCharacterBase* CharacterBase = Cast<AElementCharacterBase>(OwnerCharacter);
		CharacterBase->IsUsingUpperBodySlot = false;
	}

	EndAbility();
}

