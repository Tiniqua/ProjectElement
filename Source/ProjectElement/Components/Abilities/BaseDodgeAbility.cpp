
#include "Components/Abilities/BaseDodgeAbility.h"

#include "Actors/Character/ElementCharacterBase.h"
#include "Actors/Controllers/ElementAIControllerBase.h"


UBaseDodgeAbility::UBaseDodgeAbility()
{
}

void UBaseDodgeAbility::StartAbility()
{
	Super::StartAbility();

	if (DodgeInPlace)
	{
		PlayDodgeInPlace();
	}
	else
	{
		ChooseDodgeDirection();
	}
}

void UBaseDodgeAbility::ChooseDodgeDirection()
{
	if (OwnerCharacter == nullptr)
	{
		return;
	}

	// Get the player character
	if(const AElementAIControllerBase* AIController = Cast<AElementAIControllerBase>(OwnerCharacter->GetController()))
	{
		if(AIController->CurrentCombatTarget != nullptr)
		{
			const AElementCharacterBase* Player = Cast<AElementCharacterBase>(AIController->CurrentCombatTarget);

			if (Player == nullptr)
			{
				return;
			}

			// Determine direction to dodge based on proximity to the player
			const FVector DirectionToPlayer = (Player->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
			const FVector OwnerForward = OwnerCharacter->GetActorForwardVector();
			const FVector OwnerRight = OwnerCharacter->GetActorRightVector();

			const float ForwardDot = FVector::DotProduct(DirectionToPlayer, OwnerForward);
			const float RightDot = FVector::DotProduct(DirectionToPlayer, OwnerRight);

			UAnimSequence* DodgeAnimation;

			if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
			{
				DodgeAnimation = ForwardDot > 0 ? DodgeBackwardAnimation : DodgeForwardAnimation;
			}
			else
			{
				DodgeAnimation = RightDot > 0 ? DodgeLeftAnimation : DodgeRightAnimation;
			}

			if (IsValid(DodgeAnimation))
			{
				USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
				if (OwnerMesh != nullptr)
				{
					OwnerMesh->PlayAnimation(DodgeAnimation, false);
					IsPlayingAnimation = true;

					const float AnimationLength = DodgeAnimation->GetPlayLength();
					if (AnimationLength > 0.0f)
					{
						if (const UWorld* World = GetWorld())
						{
							World->GetTimerManager().SetTimer(AbilityMontage, this, &UBaseAbility::ResetAnimState, AnimationLength, false);
						}
					}
				}
			}
		}
	}
}

void UBaseDodgeAbility::PlayDodgeInPlace()
{
	if (IsValid(DodgeInPlaceAnimation))
	{
		USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
		if (OwnerMesh != nullptr)
		{
			OwnerMesh->PlayAnimation(DodgeInPlaceAnimation, false);
			IsPlayingAnimation = true;

			const float AnimationLength = DodgeInPlaceAnimation->GetPlayLength();
			if (AnimationLength > 0.0f)
			{
				if (const UWorld* World = GetWorld())
				{
					World->GetTimerManager().SetTimer(AbilityMontage, this, &UBaseAbility::ResetAnimState, AnimationLength, false);
				}
			}
		}
	}
}
