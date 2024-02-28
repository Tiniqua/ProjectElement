
#include "ElementAiCharacter.h"
#include "ElementCharacter.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "Actors/Misc/InteractableLootDrop.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/World/ActorPool.h"
#include "Net/UnrealNetwork.h"

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

void AElementAiCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AElementAiCharacter, SquadID);
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
	
	//GetCapsuleComponent()->SetCapsuleHalfHeight(0);
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

	SpawnLoot();
}

void AElementAiCharacter::SpawnLoot()
{
	if (LootTable.Num() == 0)
	{
		// If the loot table is empty, there's no loot to spawn
		return;
	}
	
	const int32 NumLootDrops = FMath::RandRange(3, 10);
	for (int32 i = 0; i < NumLootDrops; i++)
	{
		const int32 RandomIndex = FMath::RandRange(0, LootTable.Num() - 1);
		// Assuming LootArray is an array of loot item classes (e.g., TSubclassOf<AInteractableLootDrop>)

		if (AInteractableLootDrop* SpawnedLoot = GetWorld()->SpawnActor<AInteractableLootDrop>(LootTable[RandomIndex]))
		{
			FVector SpawnLocation = GetActorLocation();
			SpawnLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

			// Apply a random spawn offset to the loot drop
			FVector SpawnOffset = FVector(FMath::FRandRange(-100.0f, 100.0f), FMath::FRandRange(-100.0f, 100.0f), 0.0f);
			SpawnedLoot->SetActorLocation(SpawnLocation + SpawnOffset);

			SpawnedLoot->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SpawnedLoot->MeshComponent->SetSimulatePhysics(true);

			// TODO - Loot physics
		}
	}
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

void AElementAiCharacter::SetCurrentSquad(FSquadInfo* NewSquad)
{
	CurrentSquad = NewSquad;
}

FSquadInfo* AElementAiCharacter::GetCurrentSquad() const
{
	return CurrentSquad;
}

void AElementAiCharacter::SetSquadID(int NewSquadID)
{
	SquadID = NewSquadID;
}

int AElementAiCharacter::GetSquadID() const
{
	return SquadID;
}

void AElementAiCharacter::OnRep_SquadID()
{
	// TODO - UPDATE UI ABOVE AI IN WIDGET SO WE CAN VISUALISE WHICH SQUAD THEY ARE IN
	// EITHER RANDOM ICON SELECTED BASED ON SORTED INDEX AND ID OR JUST DISPLAY ID AS NUM NEXT TO HEALTHBAR
}
