
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseAbility.h"
#include "AOEStrikeAbility.generated.h"

class UBaseDamageType;
UCLASS()
class PROJECTELEMENT_API UAOEStrikeAbility : public UBaseAbility
{
	GENERATED_BODY()

	virtual void StartAbility() override;
	
	virtual void EndAbility() override;

	void CreateDamageSphere();

	UPROPERTY(EditDefaultsOnly, Category= "Debug")
	bool ShouldDrawDebug = true;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float DamageRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	float Range = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	UNiagaraSystem* DamageVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float VFXScale = 1.f;

	UPROPERTY(EditDefaultsOnly, Category= "Damage")
	TSubclassOf<UBaseDamageType> DamageType = nullptr;
	
	UPROPERTY()
	float CurrentRadius = 0.f;
	
	UPROPERTY()
	FTimerHandle CreateSphereTimerHandle;

	UPROPERTY()
	TArray<AActor*> DamagedActors;
};
