
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckDistance.generated.h"

UCLASS()
class PROJECTELEMENT_API UBTDecorator_CheckDistance : public UBTDecorator
{
	GENERATED_BODY()

	UBTDecorator_CheckDistance();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual FString GetStaticDescription() const override;

protected:

	UPROPERTY(EditAnywhere, Category=Distance, Meta=(ToolTips="Which Actor (from the blackboard) should be checked?"))
	FBlackboardKeySelector ActorToCheckTo;

	UPROPERTY(EditAnywhere, Category=Distance, Meta=(ToolTips="Minimum Distance"))
	float MinimumDistance = 0.f;

	UPROPERTY(EditAnywhere, Category=Distance, Meta=(ToolTips="Maximum Distance"))
	float MaximumDistance = 0.f;
};
