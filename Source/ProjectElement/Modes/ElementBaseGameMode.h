// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ElementBaseGameMode.generated.h"

class USpawnManager;
class UActorPool;
class AElementCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FElementGameEvent);

/** Base game mode class, implements game over states and ability to restart player and track death events. */
UCLASS()
class PROJECTELEMENT_API AElementBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FElementGameEvent OnGameOverEvent;

	/** Constructor. */
	AElementBaseGameMode();

	/** Level start. */
	virtual void BeginPlay() override;

	/** Ran when the player dies. */
	virtual void GameOver();

	/** Called after a player is spawned and possessed, bind to the on death function to keep track of player. */
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;

	UFUNCTION()
	void CharacterDied(AElementCharacterBase* InCharacterDied);
};



