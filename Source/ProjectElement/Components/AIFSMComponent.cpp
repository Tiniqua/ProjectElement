
#include "Components/AIFSMComponent.h"
#include "Actors/Character/ElementAiCharacter.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "Damage/BaseDamageType.h"

void UAIFSMComponent::RunCurrentState()
{
	Super::RunCurrentState();

	const EFSMState State = static_cast<EFSMState>(CurrentState);

	AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner());
	if (!ElementCharacter)
	{
		return;
	}

	switch (State)
	{
	case EFSMState::Spawning:
		HandleSpawningState();
		break;
	case EFSMState::Common:
		HandleCommonState();
		break;
	case EFSMState::Reaction:
		// TODO - Fix so we can have data providers for params
		// TODO - currently we call HandleReactionState(); manually externally without using proper flow
		//HandleReactionState();
		break;
	case EFSMState::Death:
		HandleDeathState(ElementCharacter);
		break;
	}
}

void UAIFSMComponent::HandleSpawningState()
{
	Super::HandleSpawningState();

	const AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner());
	if (!ElementCharacter)
	{
		return;
	}

	if(ElementCharacter->SpawnAnimMontage != nullptr)
	{
		if (UAnimMontage* SpawnAnimation = ElementCharacter->SpawnAnimMontage)
		{
			const USkeletalMeshComponent* OwnerMesh = ElementCharacter->GetMesh();
			if(OwnerMesh != nullptr)
			{
				if(UAnimInstance* Instance = OwnerMesh->GetAnimInstance(); Instance != nullptr)
				{
					const float Duration = Instance->Montage_Play(SpawnAnimation,1,EMontagePlayReturnType::Duration);
					if (Duration > 0.0f)
					{
						if(const UWorld* World = GetWorld())
						{
							World->GetTimerManager().SetTimer(SpawnAnimTimer, this, &UAIFSMComponent::OnSpawnAnimComplete, Duration, false);
							return;
						}
					}
				}
			}
		}
	}

	// If the spawn animation fails, transition to the Common state
	SetState(static_cast<uint8>(EFSMState::Common));
}


void UAIFSMComponent::HandleCommonState()
{
	Super::HandleCommonState();

	const AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner());
	if (!ElementCharacter)
	{
		return;
	}

	if(AElementAIControllerBase* Controller = ElementCharacter->GetElementAIController())
	{
		if(Controller != nullptr)
		{
			if(ElementCharacter->BehaviorTree != nullptr)
			{
				Controller->RunBehaviorTree(ElementCharacter->BehaviorTree);
			}
		}
	}
}

// TODO - Should take ReactionType enum as param for light, normal, significant reactions then switch inside
void UAIFSMComponent::HandleReactionState(EHitReactionType HitReaction)
{
	// Currently not necessary to call super as enemies implement different to characters
	// Super::HandleReactionState(HitReaction);

	// cant call hit reaction state changes the same way as normal state changes as we need data so we skip runcurrentstate()
	SetState(static_cast<uint8>(EFSMState::Reaction));

	if(HitReaction == EHitReactionType::None)
	{
		SetState(static_cast<uint8>(EFSMState::Common));
		return;
	}

	const AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner());
	if (!ElementCharacter)
	{
		SetState(static_cast<uint8>(EFSMState::Common));
		return;
	}
	
	switch (HitReaction)
	{
	case EHitReactionType::None:
		UE_LOG(LogTemp, Warning, TEXT("No Hit reaction to play"));
		SetState(static_cast<uint8>(EFSMState::Common));
		return;
	case EHitReactionType::Light:
		ReactionAnimation = ElementCharacter->ReactionAnimMontageLight;
		break;
	case EHitReactionType::Standard:
		ReactionAnimation = ElementCharacter->ReactionAnimMontageStandard;
		break;
	case EHitReactionType::Heavy:
		ReactionAnimation = ElementCharacter->ReactionAnimMontageHeavy;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No Hit reaction to play"));
		SetState(static_cast<uint8>(EFSMState::Common));
		break;
	}

	if (ReactionAnimation != nullptr)
	{
		const USkeletalMeshComponent* OwnerMesh = ElementCharacter->GetMesh();
		if(OwnerMesh != nullptr)
		{
			if(UAnimInstance* Instance = OwnerMesh->GetAnimInstance())
			{
				const float Duration = Instance->Montage_Play(ReactionAnimation,1,EMontagePlayReturnType::Duration);
				if (Duration > 0.0f)
				{
					if(AElementAIControllerBase* Controller = ElementCharacter->GetElementAIController())
					{
						if(Controller != nullptr)
						{
							Controller->SetBehaviorTreeEnabled(false);
						}
					}
					if(const UWorld* World = GetWorld())
					{
						World->GetTimerManager().SetTimer(ReactionAnimTimer, this, &UAIFSMComponent::OnReactionAnimComplete, Duration, false);
						//return;
					}
				}
			}
		}
	}

	// If the hit reaction animation fails, transition back to the Common state
	//SetState(static_cast<uint8>(EFSMState::Common));
}

void UAIFSMComponent::HandleDeathState(AElementCharacterBase* InCharacterDied)
{
	Super::HandleDeathState(InCharacterDied);

	const AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner());
	if (!ElementCharacter)
	{
		return;
	}

	if(ElementCharacter->DeathMontage != nullptr)
	{
		const USkeletalMeshComponent* OwnerMesh = ElementCharacter->GetMesh();
		if(OwnerMesh != nullptr)
		{
			if(UAnimInstance* Instance = OwnerMesh->GetAnimInstance())
			{
				const float Duration = Instance->Montage_Play(ElementCharacter->DeathMontage,1,EMontagePlayReturnType::Duration);
				if (Duration > 0.0f)
				{
					if(const UWorld* World = GetWorld())
					{
						World->GetTimerManager().SetTimer(DeathAnimTimer, this, &UAIFSMComponent::OnDeathAnimComplete, Duration, false);
					}
				}
			}
		}
	}
}

void UAIFSMComponent::OnSpawnAnimComplete()
{
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnAnimTimer);
	}
	
	SetState(static_cast<uint8>(EFSMState::Common));
}

void UAIFSMComponent::OnDeathAnimComplete()
{
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DeathAnimTimer);
	}

	const AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner());
	if (!ElementCharacter)
	{
		return;
	}
	
	if(AElementAIControllerBase* Controller = ElementCharacter->GetElementAIController())
	{
		if(Controller != nullptr)
		{
			if(Controller->GetBehaviorComp() != nullptr)
			{
				Controller->SetBehaviorTreeEnabled(false);
			}
		}
	}
	
	// TODO Move pooling and cleanup here to be ran after death anim
}

void UAIFSMComponent::OnReactionAnimComplete()
{
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ReactionAnimTimer);
	}

	if (const AElementAiCharacter* ElementCharacter = Cast<AElementAiCharacter>(GetOwner()))
	{
		if(AElementAIControllerBase* Controller = ElementCharacter->GetElementAIController())
		{
			if(Controller != nullptr)
			{
				Controller->SetBehaviorTreeEnabled(true);
			}
		}
	}

	SetState(static_cast<uint8>(EFSMState::Common));
}
