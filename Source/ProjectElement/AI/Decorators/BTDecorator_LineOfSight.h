
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_LineOfSight.generated.h"

UCLASS()
class PROJECTELEMENT_API UBTDecorator_LineOfSight : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_LineOfSight();

public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetActorKey;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
