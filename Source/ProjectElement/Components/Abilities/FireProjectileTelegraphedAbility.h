
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseTelegraphedAbility.h"
#include "Damage/BaseProjectile.h"
#include "FireProjectileTelegraphedAbility.generated.h"

UCLASS()
class PROJECTELEMENT_API UFireProjectileTelegraphedAbility : public UBaseTelegraphedAbility
{
	GENERATED_BODY()

	virtual void ActivateTelegraphedAbility() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FName MuzzleSocketName;
};
