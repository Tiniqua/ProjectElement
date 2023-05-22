// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementBaseGameMode.h"
#include "Actors/Character/ElementCharacter.h"
#include "Actors/Character/ElementCharacterBase.h"

AElementBaseGameMode::AElementBaseGameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	//...
}

void AElementBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	//...
}

void AElementBaseGameMode::CharacterDied(AElementCharacterBase* InCharacterDied)
{
	// TODO: Handle more things here for now game over is fine.
	GameOver();
}

void AElementBaseGameMode::GameOver()
{
	OnGameOverEvent.Broadcast();
}

void AElementBaseGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);

	AElementCharacter* ElementCharacter = Cast<AElementCharacter>(NewPlayer->GetCharacter());
	if (!ElementCharacter)
	{
		return;
	}

	ElementCharacter->OnCharacterDeathEvent.AddDynamic(this, &AElementBaseGameMode::CharacterDied);
}


