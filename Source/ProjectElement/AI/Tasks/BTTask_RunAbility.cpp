
#include "AI/Tasks/BTTask_RunAbility.h"
#include "AIController.h"
#include "Components/AbilityComponent.h"
#include "Components/VitalityComponent.h"

UBTTask_RunAbility::UBTTask_RunAbility(const FObjectInitializer& _objectInitializer)
{
	NodeName = "Run Ability";

	bCreateNodeInstance = true;
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_RunAbility::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	const AAIController* Controller = _ownerComp.GetAIOwner();
	if(Controller == nullptr)
	{
		return EBTNodeResult::Aborted;
	}
	
	const APawn* Pawn = Controller->GetPawn();
	if(Pawn == nullptr)
	{
		return EBTNodeResult::Aborted;
	}

	const UVitalityComponent* VitalityComponent = Pawn->FindComponentByClass<UVitalityComponent>();
	if(VitalityComponent == nullptr || VitalityComponent->GetCurrentVitality() <= 0)
	{
		return EBTNodeResult::Aborted;
	}

	const UAbilityComponent* AbilityComponent = Pawn->FindComponentByClass<UAbilityComponent>();
	if(AbilityComponent == nullptr)
	{
		return EBTNodeResult::Aborted;
	}
	
	RunTimeAbility = AbilityComponent->GetCurrentAbilityType(AbilityTypeToRun);
	if(RunTimeAbility == nullptr)
	{
		return EBTNodeResult::Aborted;
	}
	
	RunTimeAbility->StartAbility();
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_RunAbility::AbortTask(UBehaviorTreeComponent& _OwnerComp, uint8* NodeMemory)
{
	// TODO handle interrupting anim states and resetting future ABP and FSM
	
	return Super::AbortTask(_OwnerComp, NodeMemory);
}

void UBTTask_RunAbility::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// TODO - reassess this for abilities that dont use animation to drive them
	if (RunTimeAbility && !RunTimeAbility->GetIsPlayingAnimation())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

FString UBTTask_RunAbility::GetStaticDescription() const
{
	FString Description = Super::GetStaticDescription();
	Description += FString::Printf(TEXT("\nAbility Type: %s"), *AbilityTypeToRun->GetName());
	return Description;
}
