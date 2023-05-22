// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ElementAIControllerBase.generated.h"

class AElementCharacter;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class AElementAiCharacter;
class UAIPerceptionComponent;

UCLASS()
class PROJECTELEMENT_API AElementAIControllerBase : public AAIController
{
	GENERATED_BODY()

protected:

	/** Cached BehaviorTree comp. */
	UPROPERTY(Transient)
	UBehaviorTreeComponent* BehaviorTree = nullptr;

	UPROPERTY(Transient)
	AElementAiCharacter* ControlledCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent = nullptr;
	
	/** Init and deinit the blackboard and behavior tree. */
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:

	/** Constructor. */
	AElementAIControllerBase();

	/** Returns this AI's Blackboard. **/
	UBlackboardComponent* GetBlackboardComp() const { return Blackboard; }
	
	/** Returns this AI's BehaviorTree. **/
	UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorTree; }

	/** Enables/Disables the behaviour tree. */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetBehaviorTreeEnabled(bool bIsEnabled);

	/** Ran when perception is updated. */
	UFUNCTION()
	void PerceptionUpdated(AActor* InActor, FAIStimulus InStimulus);

	UPROPERTY()
	AActor* CurrentCombatTarget = nullptr;
};
