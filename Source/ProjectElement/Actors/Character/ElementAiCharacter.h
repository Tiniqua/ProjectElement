// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ElementCharacterBase.h"
#include "Components/AIFSMComponent.h"
#include "Components/World/SquadManagerComponent.h"
#include "Interfaces/PoolInterface.h"
#include "ElementAiCharacter.generated.h"

class AInteractableLootDrop;
class UBehaviorTree;
class AElementAIControllerBase;
class UActorPool;

UENUM(BlueprintType)
enum class ECombatStyle : uint8
{
	CS_Melee UMETA(DisplayName = "Melee"),
	CS_Ranged UMETA(DisplayName = "Ranged"),
	Unassigned UMETA(DisplayName = "Unassigned")
};

UENUM(BlueprintType)
enum class ERank : uint8
{
	R_Common UMETA(DisplayName = "Common"),
	R_Uncommon UMETA(DisplayName = "Uncommon"),
	R_Rare UMETA(DisplayName = "Rare"),
	R_Epic UMETA(DisplayName = "Epic"),
	R_Legendary UMETA(DisplayName = "Legendary"),
	Unassigned UMETA(DisplayName = "Unassigned")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	ECombatStyle CombatStyle = ECombatStyle::Unassigned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	ERank Rank = ERank::Unassigned;
	
protected:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "AI")
	AElementAIControllerBase* AIController = nullptr;

	UPROPERTY(Transient)
	UActorPool* OwningPool = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	TArray<TSubclassOf<AInteractableLootDrop>> LootTable;

private:
	FSquadInfo* CurrentSquad = nullptr;

	// ----------- BINDING USING ON REP TO UPDATE UI ----------------
	UPROPERTY(ReplicatedUsing = OnRep_SquadID)
	int SquadID = -1;
	
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

	// TODO - From datatable or archetype data?
	void SpawnLoot();

	UFUNCTION()
	void Cleanup();

	// squad data
	void SetCurrentSquad(FSquadInfo* NewSquad);
	FSquadInfo* GetCurrentSquad() const;
	
	void SetSquadID(int NewSquadID);
	int GetSquadID() const;

	UFUNCTION()
	static void OnRep_SquadID();

	AElementAIControllerBase* GetElementAIController() const {return AIController;};
};