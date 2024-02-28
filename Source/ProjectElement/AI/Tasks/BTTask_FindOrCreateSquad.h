
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindOrCreateSquad.generated.h"

UCLASS()
class PROJECTELEMENT_API UBTTask_FindOrCreateSquad : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;

	UPROPERTY(EditAnywhere)
	float MaxRange = 5000.f;

	UPROPERTY(EditAnywhere)
	bool ShouldDrawDebug = false;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKey;
};
