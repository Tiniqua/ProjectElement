// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ElementCharacterBase.h"
#include "Components/AIFSMComponent.h"
#include "Interfaces/PoolInterface.h"
#include "ElementAiCharacter.generated.h"

class UBehaviorTree;
class AElementAIControllerBase;
class UActorPool;

UCLASS(Abstract, ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent))
class PROJECTELEMENT_API AElementAiCharacter : public AElementCharacterBase, public IPoolInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAnimMontage* SpawnAnimMontage;

	// TODO - Expand in future to be driven by damage received
	// TODO - Also support additive reaction sequences for less significant damage rather than montages
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAnimMontage* ReactionAnimMontageLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAnimMontage* ReactionAnimMontageStandard;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAnimMontage* ReactionAnimMontageHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float DeathDespawnTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAIFSMComponent* AIFSMComponent = nullptr;
	
protected:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "AI")
	AElementAIControllerBase* AIController = nullptr;

	UPROPERTY(Transient)
	UActorPool* OwningPool = nullptr;
	
public:

	/** Constructor. */
	AElementAiCharacter();

	virtual void PostInitializeComponents() override;
	
	// ~ Begin IPoolInterface
	virtual void PoolInitialised_Implementation(UActorPool* InPool) override; 
	virtual void PoolOnSpawned_Implementation(UActorPool* InPool) override; 
	virtual void PoolOnDespawned_Implementation(UActorPool* InPool) override; 
	// ~ End IPoolInterface

	/** Handle enemy death event. */
	virtual void OnDeath() override;

	UFUNCTION()
	void Cleanup();

	AElementAIControllerBase* GetElementAIController() const {return AIController;};
};