#include "Modes/SquadManager.h"

ASquadManager::ASquadManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SquadManagerComponent = CreateDefaultSubobject<USquadManagerComponent>(TEXT("SquadManagerComponent"));
}

void ASquadManager::BeginPlay()
{
	Super::BeginPlay();
}

void ASquadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

