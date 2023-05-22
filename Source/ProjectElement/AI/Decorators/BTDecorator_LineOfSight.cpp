
#include "AI/Decorators/BTDecorator_LineOfSight.h"
#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_LineOfSight::UBTDecorator_LineOfSight()
{
	NodeName = "Check Line of Sight";
}

bool UBTDecorator_LineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	const AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (!ensure(OwnerController != nullptr))
	{
		return false;
	}

	APawn* OwnerPawn = OwnerController->GetPawn();
	if (!ensure(OwnerPawn != nullptr))
	{
		return false;
	}
	
	const UObject* TargetKeyValue = BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName);
	
	if (TargetKeyValue == nullptr)
	{
		return false;
	}

	const AActor* TargetActor = Cast<const AActor>(TargetKeyValue);
	if (!ensure(TargetActor != nullptr))
	{
		return false;
	}

	UWorld* World = OwnerPawn->GetWorld();
	if (!World)
	{
		return false;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerPawn);

	// Can enable debug draw using EDrawDebugType for duration
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(World, OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		// Check if the hit actor is the same as the target actor
		if (HitResult.GetActor() == TargetActor)
		{
			// There is a direct line of sight
			return true;
		}
	}

	// No direct line of sight
	return false;
}

FString UBTDecorator_LineOfSight::GetStaticDescription() const
{
	return FString::Printf(
		TEXT("Checking Line of Sight to %s"),
			*TargetActorKey.SelectedKeyName.ToString()
			);
}
