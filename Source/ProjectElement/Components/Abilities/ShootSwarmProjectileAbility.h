#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseShootProjectileAbility.h"
#include "ShootSwarmProjectileAbility.generated.h"

UCLASS()
class PROJECTELEMENT_API UShootSwarmProjectileAbility : public UBaseShootProjectileAbility
{
	GENERATED_BODY()

public:
	virtual void StartAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swarm")
	int32 NumberOfProjectiles = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swarm")
	bool IsHomingProjectile = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Swarm")
	float ConeAngle = 0.f;
};
