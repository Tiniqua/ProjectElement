

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindCombatTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTELEMENT_API UBTService_FindCombatTarget : public UBTService
{
	GENERATED_BODY()

	UBTService_FindCombatTarget();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKey;
};
