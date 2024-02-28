#include "AI/Tasks/BTTask_FindOrCreateSquad.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Actors/Character/ElementAiCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Modes/ElementBaseGameMode.h"
#include "Modes/SquadManager.h"


EBTNodeResult::Type UBTTask_FindOrCreateSquad::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	const AAIController* AIController = _ownerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		return EBTNodeResult::Aborted;
	}

	const UWorld* World = AIController->GetWorld();
	if (!World)
	{
		return EBTNodeResult::Aborted;
	}

	APawn* OwnerPawn = AIController->GetPawn();
	if (!ensure(OwnerPawn != nullptr))
	{
		return EBTNodeResult::Aborted;
	}

	if (ShouldDrawDebug)
	{
		const FVector OwnerLocation = OwnerPawn->GetActorLocation();
		DrawDebugSphere(World, OwnerLocation, MaxRange, 32, FColor::Green, false, 3.0f, 0, 1.0f);
	}

	TArray<AActor*> AIActors;
	UGameplayStatics::GetAllActorsOfClass(World, AElementAiCharacter::StaticClass(), AIActors);

	const AElementBaseGameMode* GameMode = Cast<AElementBaseGameMode>(World->GetAuthGameMode());
	if (!GameMode || !GameMode->SquadManagerActor)
	{
		return EBTNodeResult::Aborted;
	}

	const ASquadManager* SquadManager = Cast<ASquadManager>(GameMode->SquadManagerActor);
	if (!SquadManager || !SquadManager->SquadManagerComponent)
	{
		return EBTNodeResult::Aborted;
	}

	if(USquadManagerComponent* SquadManagerComponent = SquadManager->SquadManagerComponent)
	{
		for (AActor* Actor : AIActors)
		{
			// IGNORE SELF
			if (Actor->GetUniqueID() != OwnerPawn->GetUniqueID())
			{
				if (AElementAiCharacter* AICharacter = Cast<AElementAiCharacter>(Actor))
				{
					// Check if the AI character is within MaxRange
					const float Distance = FVector::Dist(OwnerPawn->GetActorLocation(), AICharacter->GetActorLocation());
					if (Distance <= MaxRange)
					{
						// Check if the AI character already has a squad
						if (FSquadInfo* Squad = AICharacter->GetCurrentSquad())
						{                        
							if (Squad->CanAddMembers && Squad->CurrentSquadSize < Squad->MaxSquadSize)
							{
								// Add the AI character to the squad
								SquadManagerComponent->AddMemberToSquad(AICharacter, *Squad);
                        
								return EBTNodeResult::Succeeded;
							}
						}
					}
				}
			}
		}

		if(AElementCharacterBase* OwningCharacter = Cast<AElementCharacterBase>(OwnerPawn))
		{
			SquadManagerComponent->CreateSquad(OwningCharacter);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
