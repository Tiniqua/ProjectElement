
#include "Components/Abilities/ShootSwarmProjectileAbility.h"

#include "Actors/Character/ElementCharacterBase.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UShootSwarmProjectileAbility::StartAbility()
{
	if (!ProjectileClass)
	{
		return;
	}

	if (const AElementCharacterBase* Character = Cast<AElementCharacterBase>(GetOwner()))
	{
		const FVector SpawnLocation = Character->GetActorLocation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FRotator TargetRotation = Character->GetActorForwardVector().Rotation();


		const AElementAIControllerBase* AIController = Cast<AElementAIControllerBase>(Character->GetController());
		if(AIController)
		{
			if(AIController->CurrentCombatTarget != nullptr)
			{
				const FVector DirectionVector = AIController->CurrentCombatTarget->GetActorLocation() - Character->GetActorLocation();
				TargetRotation = DirectionVector.Rotation();
			}
		}

		for (int32 i = 0; i < NumberOfProjectiles; ++i)
		{
			const float RandomYawOffset = FMath::RandRange(-ConeAngle, ConeAngle);
			const float RandomPitchOffset = FMath::RandRange(-ConeAngle, ConeAngle);

			FRotator ProjectileRotation = TargetRotation + FRotator(RandomPitchOffset, RandomYawOffset, 0.0f);
			ABaseProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, ProjectileRotation, SpawnParams);

			if(IsHomingProjectile && SpawnedProjectile && AIController->CurrentCombatTarget->GetRootComponent())
			{
				USceneComponent* Target = Cast<USceneComponent>(AIController->CurrentCombatTarget->GetRootComponent());
				
				SpawnedProjectile->ProjectileMovementComponent->bIsHomingProjectile = true;
				SpawnedProjectile->ProjectileMovementComponent->HomingTargetComponent = Target;
				SpawnedProjectile->SourceActor = GetOwner();
			}
		}
	}
}
