
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseAbility.h"
#include "Damage/BaseProjectile.h"
#include "BaseShootProjectileAbility.generated.h"

UCLASS()
class PROJECTELEMENT_API UBaseShootProjectileAbility : public UBaseAbility
{
	GENERATED_BODY()

public:
	virtual void StartAbility() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FName MuzzleSocketName;
};
