
#include "Components/Abilities/FireProjectileTelegraphedAbility.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Actors/Controllers/ElementAIControllerBase.h"


void UFireProjectileTelegraphedAbility::ActivateTelegraphedAbility()
{
	if (!ProjectileClass)
	{
		return;
	}

	if (const AElementCharacterBase* Character = Cast<AElementCharacterBase>(GetOwner()))
	{
		// TODO - Move to muzzle location when support for weapon holding exists
		const FVector SpawnLocation = Character->GetActorLocation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		const FVector UpdatedLoc = FVector(CurrentAbilityLocation.X,CurrentAbilityLocation.Y,SpawnLocation.Z);
		const FVector TargetVector = UpdatedLoc - SpawnLocation;
		
		if(ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, TargetVector.Rotation(), SpawnParams))
		{
			SpawnedProjectile->SourceActor = GetOwner();
		}
	}

	Super::EndAbility();
}
