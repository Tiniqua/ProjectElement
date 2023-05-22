

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindCombatTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTELEMENT_API UBTTask_FindCombatTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKey;
};
