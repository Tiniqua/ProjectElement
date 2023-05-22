
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/Abilities/BaseAbility.h"
#include "BTTask_RunAbility.generated.h"

UCLASS()
class PROJECTELEMENT_API UBTTask_RunAbility : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_RunAbility(const FObjectInitializer& _objectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& _OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual FString GetStaticDescription() const override;

public:
	UPROPERTY(EditAnywhere, Category="Ability")
	TSubclassOf<UBaseAbility> AbilityTypeToRun;

	UPROPERTY()
	UBaseAbility* RunTimeAbility;
};
