
#include "ElementAiCharacter.h"
#include "ElementCharacter.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/World/ActorPool.h"

AElementAiCharacter::AElementAiCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AElementAIControllerBase::StaticClass();
	AIFSMComponent = CreateDefaultSubobject<UAIFSMComponent>(TEXT("AIFSMComponent"));
}

void AElementAiCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIController = Cast<AElementAIControllerBase>(GetController());
	if (!AIController)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: EnemyCharacter %s has no AIController, it will not function correctly!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
	}
}

void AElementAiCharacter::PoolInitialised_Implementation(UActorPool* InPool)
{
	// Disable the behavior tree until it is use.
	if (AIController)
	{
		AIController->SetBehaviorTreeEnabled(false);
	}

	OwningPool = InPool;
}

void AElementAiCharacter::PoolOnSpawned_Implementation(UActorPool* InPool)
{
	// Enable the behavior tree when it is needed.
	if (AIController)
	{
		AIController->SetBehaviorTreeEnabled(true);
	}
}

void AElementAiCharacter::PoolOnDespawned_Implementation(UActorPool* InPool)
{
	// Disable the behavior tree when this enemy is returned to the pool.
	if (AIController)
	{
		AIController->SetBehaviorTreeEnabled(true);
	}

	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AElementAiCharacter::OnDeath()
{
	Super::OnDeath();

	if(AIFSMComponent != nullptr)
	{
		// handles death anim - TODO - has a callback func for after death anim played where pooling should be moved to in future
		AIFSMComponent->SetState(static_cast<uint8>(EFSMState::Death));
	}
	
	if (CharacterMesh)
	{
		CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CharacterMesh->SetSimulatePhysics(true);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AIController)
	{
		AIController->SetBehaviorTreeEnabled(false);
		
		// TODO - Renable brain when unpooled
		AIController->BrainComponent = nullptr;
	}
	
	auto RemoveActor = ([this]
	{
		if (OwningPool)
		{
			OwningPool->DespawnActorInPool(this);
			return;
		}

		Destroy();
	});

	FTimerHandle FallbackDeathHandle;
	FTimerDelegate FallbackDeathDelegate;
	FallbackDeathDelegate.BindUFunction(this, "Cleanup", false);
	GetWorld()->GetTimerManager().SetTimer(FallbackDeathHandle, FallbackDeathDelegate, DeathDespawnTime, false, DeathDespawnTime);
}

// TODO - Temp until pooling setup
void AElementAiCharacter::Cleanup()
{
	// Add Death Material fade and VFX?
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();
}
