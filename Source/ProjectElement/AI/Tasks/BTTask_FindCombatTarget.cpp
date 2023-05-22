
#include "AI/Tasks/BTTask_FindCombatTarget.h"
#include "AIController.h"
#include "Actors/Character/ElementCharacter.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_FindCombatTarget::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
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

	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(World, AElementCharacter::StaticClass(), PlayerActors);

	AActor* NearestPlayer = nullptr;
	float MinDistance = MAX_flt;

	for (AActor* Player : PlayerActors)
	{
		const float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), Player->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestPlayer = Player;
		}
	}

	if (NearestPlayer)
	{
		if(AElementAIControllerBase* AIControllerBase = Cast<AElementAIControllerBase>(_ownerComp.GetAIOwner()))
		{
			AIControllerBase->CurrentCombatTarget = NearestPlayer;
		}
		
		_ownerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, NearestPlayer);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
