


#include "AI/Services/BTService_FindCombatTarget.h"

#include "AIController.h"
#include "Actors/Character/ElementCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindCombatTarget::UBTService_FindCombatTarget()
{
	NodeName = "Find Combat Target";
}

void UBTService_FindCombatTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		return;
	}

	const UWorld* World = AIController->GetWorld();
	if (!World)
	{
		return;
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
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, NearestPlayer);
	}
}
