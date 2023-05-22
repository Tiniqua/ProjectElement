// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolInterface.generated.h"

class UActorPool;

// This class does not need to be modified.
UINTERFACE()
class UPoolInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTELEMENT_API IPoolInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actor Pool")
	void PoolInitialised(UActorPool* InPool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actor Pool")
	void PoolOnSpawned(UActorPool* InPool);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actor Pool")
	void PoolOnDespawned(UActorPool* InPool);

	virtual void PoolInitialised_Implementation(UActorPool* InPool); /** Called when an actor is spawned and initialised into an actor pool. */
	virtual void PoolOnSpawned_Implementation(UActorPool* InPool); /** Called when an actor is spawned from a pool. */
	virtual void PoolOnDespawned_Implementation(UActorPool* InPool); /** Called when an actor is de-spawned and returned to the pool. */
};
