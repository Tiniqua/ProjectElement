// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPoolEvent, AActor*, PooledActor);

class AActor;

/** Condition functionality in the event the pool is empty and something is trying to spawn an item from the actor pool. */
UENUM(BlueprintType)
enum class EPoolEmptyCondition : uint8
{
	SpawnNewActor,
	ReuseActor,
	DoNothing
};

/** Actor pool implementation for spawning existing actors in the world for re-use during runtime.
 *  NOTE: Run the InitialisePool manually to setup the pool during initialisation phases on the actor using this component.
 *  NOTE: If bInitialiseManually it will be created on begin play, if no name is set it will be set to the name of the pool component.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTELEMENT_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Should have to be initalised manually? If false it will be initialised on begin play. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Pool")
	bool bInitialiseManually = true;

	/** New actor is spawned in the world and initialised into this pool. */
	UPROPERTY(BlueprintAssignable)
	FOnPoolEvent OnPoolInitialisedActor;
	
	/** Actor inside the pool has been spawned and is now in-use. */
	UPROPERTY(BlueprintAssignable)
	FOnPoolEvent OnPoolSpawnedActor;

	/** Actor being used in the world owned by this pool has been moved back into the pool and is no longer active. */
	UPROPERTY(BlueprintAssignable)
	FOnPoolEvent OnPoolDespawnActor;

public:
	
	/** Constructor. */
	UActorPool();

	/** Level start. */
	virtual void BeginPlay() override;

	/** Initialise the pool with the current layout. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	bool InitialisePool();
	
	/** Initialise the pool with a given subclass. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	bool InitialisePoolWithData(TSubclassOf<AActor> InPoolType, FName InPoolName, int32 PoolSize = 25, EPoolEmptyCondition InEmptyCondition = EPoolEmptyCondition::DoNothing);

	/** Spawn an actor in the world at a given transform and assign owner/instigator etc. from the pool. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	AActor* SpawnActorFromPool(FVector InSpawnLocation, FRotator InSpawnRotation, AActor* InOwner, APawn* InInstigator);

	/** De-spawn a given actor back into the pool if contained in this pool. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	void DespawnActorInPool(AActor* InActor);

	/** Destroys all actors in the pool and resets the pool entirely.
	 *  NOTE: If you run this you will have to InitialisePool manually for it to be usable again... */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	void ClearPool();

	/** Reset the pool in terms of in world pooled objects. (De-spawn all pooled actors in world.) */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	void ResetPool();

	/** Is the actor contained/owned by this actor pool. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	bool PoolContains(AActor* InActor) const;

	/** What is the current size of the pool? */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	int32 GetPoolCount() const { return CurrentPoolSize; }

	/** How many actors are currently active that are owned by this pool. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	int32 GetWorldActorsCount() const { return InWorld.Num(); }

	/** How many actors are in-active and not in use. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	int32 GetPooledActorsCount() const { return InPool.Num(); }

	/** How many actors exist in total in this pool. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	int32 GetTotalPooledActorsCount() const { return InPool.Num() + InWorld.Num(); }

	/** Does the pool have available actors that can be spawned? */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	bool PoolHasAvailableActors() const { return InPool.Num() > 0; }

	/** Get the pools actors that are active within the world. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	TArray<AActor*> GetActorsInPool() const { return InPool; }

	/** Get the pools actors that are currently not in-use. */
	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	TArray<AActor*> GetActorsInWorld() const { return InWorld; }

	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	FName GetPoolName() const { return PoolName; }

private:

	/** Start the pool, depending on bInitialiseManually this can be done on begin play or from InitialisePool. */
	void StartPool();

	/** Spawn an actor in the world and initialise it into this pool. */
	bool AddActorIntoPool();

protected:
	
	/** Actor to be pooled. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Pool")
	TSubclassOf<AActor> ActorToPool;

	/** Default pool size. (Pool will be increase with warning messages if required by system using the pool). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor Pool")
	int32 DefaultSize = 25;

	/** Conditional functionality in the event the pool attempts to spawn an actor and the pool is empty. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Pool")
	EPoolEmptyCondition EmptyCondition = EPoolEmptyCondition::DoNothing;

	/** Name of this pool. Given when initialised. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Pool")
	FName PoolName = NAME_None;

	/** Actors in pool but not in-use. */
	UPROPERTY()
	TArray<AActor*> InPool;

	/** Actors in pool that are in-use. */
	UPROPERTY()
	TArray<AActor*> InWorld;

private:

	/** Size of the current pool. */
	int32 CurrentPoolSize = 0;
};
