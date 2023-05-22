
#include "Components/FSMComponent.h"
#include "Actors/Character/ElementCharacterBase.h"

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	const AElementCharacterBase* ElementCharacter = Cast<AElementCharacterBase>(GetOwner());
	if (!ElementCharacter)
	{
		return;
	}
	
	SetState(static_cast<uint8>(EFSMState::Spawning));
}

void UFSMComponent::HandleSpawningState()
{
	// Char specific setup
}

void UFSMComponent::HandleCommonState()
{
	// Char specific setup
}

void UFSMComponent::HandleReactionState(EHitReactionType HitReaction)
{
	
}

void UFSMComponent::HandleDeathState(AElementCharacterBase* InCharacterDied)
{
	// Char specific setup
}

void UFSMComponent::SetState(uint8 NewState)
{
	CurrentState = NewState;
	RunCurrentState();
}

void UFSMComponent::RunCurrentState()
{
	// Char specific setup
}
