
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseTelegraphedAbility.h"
#include "AOETelegraphAbility.generated.h"

class UBaseDamageType;
UCLASS()
class PROJECTELEMENT_API UAOETelegraphAbility : public UBaseTelegraphedAbility
{
	GENERATED_BODY()
	
	virtual void ActivateTelegraphedAbility() override;

public:
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DamageRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	TSubclassOf<UBaseDamageType> DamageType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	UNiagaraSystem* DamageVFX = nullptr;
};
