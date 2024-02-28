

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasSquad.generated.h"


UCLASS()
class PROJECTELEMENT_API UBTDecorator_HasSquad : public UBTDecorator
{
	GENERATED_BODY()
	UBTDecorator_HasSquad();
	
public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetActorKey;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;
};
