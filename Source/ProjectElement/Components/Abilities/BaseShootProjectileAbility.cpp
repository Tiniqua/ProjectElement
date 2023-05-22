
#include "Components/Abilities/BaseShootProjectileAbility.h"

#include "Actors/Character/ElementCharacterBase.h"
#include "Actors/Controllers/ElementAIControllerBase.h"

void UBaseShootProjectileAbility::StartAbility()
{
	Super::StartAbility();

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

		// By default, use the character's forward vector for the rotation
		FRotator TargetRotation = Character->GetActorForwardVector().Rotation();

		if(const AElementAIControllerBase* AIController = Cast<AElementAIControllerBase>(Character->GetController()))
		{
			if(AIController->CurrentCombatTarget != nullptr)
			{
				// Calculate the vector between the character and the combat target
				const FVector DirectionVector = AIController->CurrentCombatTarget->GetActorLocation() - Character->GetActorLocation();

				// Get the rotation of the direction vector
				TargetRotation = DirectionVector.Rotation();
			}
		}

		// TODO - Needs to be done seperately so child classes can call super and have access without having to reimplement within
		if(ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, TargetRotation, SpawnParams))
		{
			SpawnedProjectile->SourceActor = GetOwner();
		}
	}
}
