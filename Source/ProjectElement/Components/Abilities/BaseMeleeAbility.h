
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseAbility.h"
#include "BaseMeleeAbility.generated.h"

class UBaseDamageType;
UCLASS()
class PROJECTELEMENT_API UBaseMeleeAbility : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void StartAbility() override;

	UPROPERTY(EditDefaultsOnly, Category= "Debug")
	bool ShouldDrawDebug = true;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DamageRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	TSubclassOf<UBaseDamageType> DamageType = nullptr;
};
