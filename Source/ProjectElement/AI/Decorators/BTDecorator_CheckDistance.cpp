
#include "AI/Decorators/BTDecorator_CheckDistance.h"
#include <AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = "Check Distance";
	
	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	const APawn* OwnerPawn = OwnerController->GetPawn();
	if (!ensure(OwnerPawn != nullptr))
	{
		return false;
	}
	
	const UObject* TargetKeyValue = BlackboardComp->GetValueAsObject(ActorToCheckTo.SelectedKeyName);
	
	if (TargetKeyValue == nullptr)
	{
		return false;
	}

	const AActor* TargetActor = Cast<const AActor>(TargetKeyValue);
	if (!ensure(TargetActor != nullptr))
	{
		return false;
	}

	const FVector ActorFromLocation = OwnerPawn->GetActorLocation();
	const FVector ActorToLocation = TargetActor->GetActorLocation();
	
	const float DistanceBetween = UKismetMathLibrary::Vector_Distance(ActorFromLocation,ActorToLocation);
	if (DistanceBetween >= MinimumDistance && DistanceBetween <= MaximumDistance)
	{
		return true;
	}

	return false;
}

FString UBTDecorator_CheckDistance::GetStaticDescription() const
{
	return FString::Printf(TEXT("Checking distance to %s within range [%.1f, %.1f]"),
		*ActorToCheckTo.SelectedKeyName.ToString(),
		FMath::RoundToFloat(MinimumDistance * 10.0f) / 10.0f,
		FMath::RoundToFloat(MaximumDistance * 10.0f) / 10.0f
		);
}
