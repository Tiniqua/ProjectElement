// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementAIControllerBase.h"
#include "Actors/Character/ElementAiCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AElementAIControllerBase::AElementAIControllerBase()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"), true);
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	BrainComponent = BehaviorTree;
}

void AElementAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Initialise the behaviour.
	ControlledCharacter = Cast<AElementAiCharacter>(InPawn);
	if (!ControlledCharacter)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: %s, Failed to find AElementAIControllerBase controlled pawn. Cannot setup behaviour tree!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}

	UBehaviorTree* FoundBehaviorTree = ControlledCharacter->BehaviorTree;
	if (!FoundBehaviorTree)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: Failed to find reference to BehaviorTree in controlled character %s."), ANSI_TO_TCHAR(__FUNCTION__), *ControlledCharacter->GetName());
		return;
	}

	UBlackboardData* FoundBlackboardData = FoundBehaviorTree->BlackboardAsset;
	if (!FoundBlackboardData)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: BehaviorTree %s has no valid blackboard data? Cannot initialsie AI BehaviorTree."), ANSI_TO_TCHAR(__FUNCTION__), *FoundBehaviorTree->GetName());
		return;
	}

	Blackboard->InitializeBlackboard(*FoundBlackboardData);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AElementAIControllerBase::PerceptionUpdated);
}

void AElementAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	if (BehaviorTree)
	{
		BehaviorTree->StopTree();
	}
}

void AElementAIControllerBase::SetBehaviorTreeEnabled(bool bIsEnabled)
{
	if (!bIsEnabled)
	{
		if (BehaviorTree)
		{
			BehaviorTree->StopTree();
		}

		return;
	}

	if (!ControlledCharacter)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: AIController %s has no reference to an active AElementAIControllerBase?! Cannot start the behavior tree!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}
	
	BehaviorTree->StartTree(*(ControlledCharacter->BehaviorTree));
}

void AElementAIControllerBase::PerceptionUpdated(AActor* InActor, FAIStimulus InStimulus)
{
	if (!Blackboard)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: AIController %s cannot act on perception as there is no reference to a blackboard!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}

	//...
}