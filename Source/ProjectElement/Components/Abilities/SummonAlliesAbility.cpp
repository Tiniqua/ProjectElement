


#include "Components/Abilities/SummonAlliesAbility.h"
#include "Actors/Character/ElementAiCharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "NiagaraFunctionLibrary.h"

void USummonAlliesAbility::StartAbility()
{
	Super::StartAbility();

	if (!OwnerCharacter || !SpawnLocationsQuery)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(World);
	if (!QueryManager)
	{
		return;
	}
	
	FEnvQueryRequest QueryRequest(SpawnLocationsQuery, OwnerCharacter);
	QueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &USummonAlliesAbility::OnQueryFinished);
}

void USummonAlliesAbility::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	if(Result != nullptr)
	{
		if (Result->IsFinished() && Result->Items.Num() > 0)
		{
			for (int32 i = 0; i < EnemyCount; ++i)
			{
				FVector SpawnLocation = Result->GetItemAsLocation(FMath::RandRange(0,Result->Items.Num()-1));;
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = OwnerCharacter;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				if(SummonLocationVFX != nullptr)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, SummonLocationVFX, SpawnLocation);
				}
				
				// store ref in case we want to access spawned enemies
				AElementAiCharacter* SpawnedEnemy = World->SpawnActor<AElementAiCharacter>(EnemyType, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			}
		}
	}
}
