
#include "AI/Decorators/BTDecorator_HasSquad.h"
#include <AIController.h>
#include "Actors/Character/ElementAiCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_HasSquad::UBTDecorator_HasSquad()
{
	NodeName = "In Squad?";
	
}

bool UBTDecorator_HasSquad::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	const AElementAiCharacter* AICharacter = Cast<const AElementAiCharacter>(OwnerPawn);
	if (!ensure(AICharacter != nullptr))
	{
		return false;
	}

	// valid squadData means we are in a squad
	if(AICharacter->GetCurrentSquad())
	{
		return true;
	}
	
	return false;
}

FString UBTDecorator_HasSquad::GetStaticDescription() const
{
	return FString::Printf(TEXT("Targets Self Pawn"));
}
