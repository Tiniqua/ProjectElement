// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "Interfaces/PoolInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorPool, Warning, All);

UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	if (bInitialiseManually)
	{
		return;
	}

	InitialisePool();
}

bool UActorPool::InitialisePool()
{
	if (!ActorToPool)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Cannot create pool %s, there is no actor to pool set!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}

	if (DefaultSize <= 0)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Cannot create pool %s, the default size is 0 or less!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}

	if (PoolName == NAME_None)
	{
		PoolName = FName(GetName());
	}

	CurrentPoolSize = DefaultSize;
	StartPool();
	return true;
}

bool UActorPool::InitialisePoolWithData(TSubclassOf<AActor> InPoolType, FName InPoolName, int32 PoolSize, EPoolEmptyCondition InEmptyCondition)
{
	if (!InPoolType)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Cannot initialise pool %s the pool type class is null?!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}

	if (PoolSize == 0)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Cannot initalise pool %s the pool size is 0?!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}

	ActorToPool = InPoolType;
	DefaultSize = PoolSize;
	CurrentPoolSize = DefaultSize;
	EmptyCondition = InEmptyCondition;
	PoolName = InPoolName;
	StartPool();

	if (GetTotalPooledActorsCount() <= 0)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Actor pool %s size after initialsiation is 0?"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}
	
	return true;
}

void UActorPool::StartPool()
{
	ClearPool();
	
	for (int32 i = 0; i < DefaultSize; i++)
	{
		AddActorIntoPool();
	}
}

bool UActorPool::AddActorIntoPool()
{
	if (!ActorToPool)
	{
		return false;
	}

	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: World is null?! Cannot add actor to pool %s!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* NewActor = CurrentWorld->SpawnActor<AActor>(ActorToPool, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	if (!NewActor)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Spawned actor is null?! Using subclass: %s"), ANSI_TO_TCHAR(__FUNCTION__), *ActorToPool->GetName());
		return false;
	}

	NewActor->SetActorHiddenInGame(true);
	NewActor->SetActorEnableCollision(false);
	NewActor->SetActorTickEnabled(false);
	
	InPool.Add(NewActor);

	if (NewActor->GetClass()->ImplementsInterface(UPoolInterface::StaticClass()))
	{
		IPoolInterface::Execute_PoolInitialised(NewActor, this);
	}
	
	OnPoolInitialisedActor.Broadcast(NewActor);
	return true;
}

AActor* UActorPool::SpawnActorFromPool(FVector InSpawnLocation, FRotator InSpawnRotation, AActor* InOwner, APawn* InInstigator)
{
	if (!PoolHasAvailableActors())
	{
		switch (EmptyCondition)
		{
		case EPoolEmptyCondition::SpawnNewActor:
		{
			if (!AddActorIntoPool())
			{
				return nullptr;
			}
		}
		break;
		case EPoolEmptyCondition::ReuseActor: // Remove oldest item from world for re-use.
		{
			if (InWorld.Num() <= 0)
			{
				UE_LOG(LogActorPool, Warning, TEXT("%s: No actors to reuse from world in pool %s!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
				return nullptr;
			}
				
			DespawnActorInPool(InWorld[0]);
		}
		break;
		case EPoolEmptyCondition::DoNothing:
		{
			UE_LOG(LogActorPool, Warning, TEXT("%s: Cannot spawn actor from pool %s, ran out of space!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
			return nullptr;
		}
		default: ;
		}
	}

	AActor* SelectActor = InPool.Top();
	if (!SelectActor)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Selected pool actor in array is null?! Pool: %s"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return nullptr;
	}
	
	SelectActor->SetActorLocationAndRotation(InSpawnLocation, InSpawnRotation);
	SelectActor->SetOwner(InOwner);
	SelectActor->SetInstigator(InInstigator);
	SelectActor->SetActorHiddenInGame(false);
	SelectActor->SetActorEnableCollision(true);
	SelectActor->SetActorTickEnabled(true);

	InPool.Remove(SelectActor);
	InWorld.Add(SelectActor);

	if (SelectActor->GetClass()->ImplementsInterface(UPoolInterface::StaticClass()))
	{
		IPoolInterface::Execute_PoolOnSpawned(SelectActor, this);
	}

	OnPoolSpawnedActor.Broadcast(SelectActor);
	return SelectActor;
}

void UActorPool::DespawnActorInPool(AActor* InActor)
{
	if (!InActor)
	{
		UE_LOG(LogActorPool, Error, TEXT("%s: Trying to despawn a null actor in the pool %s?"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}

	if (InWorld.Num() <= 0 || !InWorld.Contains(InActor))
	{
		return;
	}

	InActor->SetActorHiddenInGame(true);
	InActor->SetActorEnableCollision(false);
	InActor->SetActorTickEnabled(false);

	InWorld.Remove(InActor);
	InPool.Add(InActor);

	if (InActor->GetClass()->ImplementsInterface(UPoolInterface::StaticClass()))
	{
		IPoolInterface::Execute_PoolOnDespawned(InActor, this);
	}
		
	OnPoolDespawnActor.Broadcast(InActor);
}

void UActorPool::ClearPool()
{
	if (InPool.Num() > 0)
	{
		for (AActor* PooledActor : InPool)
		{
			if (!PooledActor)
			{
				continue;
			}

			PooledActor->Destroy();
		}

		InPool.Empty();
	}

	if (InWorld.Num() > 0)
	{
		for (AActor* PooledActor : InWorld)
		{
			if (!PooledActor)
			{
				continue;
			}

			PooledActor->Destroy();
		}

		InWorld.Empty();
	}
}

void UActorPool::ResetPool()
{
	if (InWorld.Num() <= 0)
	{
		return;
	}
	
	for (AActor* PooledActor : InWorld)
	{
		DespawnActorInPool(PooledActor);
	}
}

bool UActorPool::PoolContains(AActor* InActor) const
{
	if (!InActor || !ActorToPool || (ActorToPool->GetClass() != InActor->GetClass()))
	{
		return false;
	}
	
	if (InPool.Contains(InActor))
	{
		return true;
	}

	if (InWorld.Contains(InActor))
	{
		return true;
	}

	return false;
}