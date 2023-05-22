
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseAbility.h"
#include "ShootBeamAbility.generated.h"

class UNiagaraComponent;
class UBaseDamageType;
UCLASS()
class PROJECTELEMENT_API UShootBeamAbility : public UBaseAbility
{
	GENERATED_BODY()

	UShootBeamAbility();
	
	virtual void StartAbility() override;
	virtual void EndAbility() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TraceAndDealDamage();

	UFUNCTION()
	void UpdateBeam(float DeltaTime);
	
	UPROPERTY(EditDefaultsOnly, Category= "Debug")
	bool ShouldDrawDebug = true;

	UPROPERTY(Transient)
	UNiagaraComponent* BeamComponent = nullptr;

	UPROPERTY(Transient)
	FVector BeamStart = FVector::ZeroVector;
	UPROPERTY(Transient)
	FVector BeamCurrentEnd = FVector::ZeroVector;
	UPROPERTY(Transient)
	FVector BeamTargetEnd = FVector::ZeroVector;

	UPROPERTY(Transient)
	bool IsBeamActive = false;

	UPROPERTY(EditDefaultsOnly, Category= "Data")
	FName SocketName = FName("");

	UPROPERTY(EditDefaultsOnly, Category= "Data")
	float LerpSpeed = 0.5f;
	
	FTimerHandle TimerHandle_DealDamage;
	UPROPERTY(EditDefaultsOnly, Category= "Data")
	float DamageInterval = 0.2f;

	FTimerHandle TimerHandle_BeamDuration;
	UPROPERTY(EditDefaultsOnly, Category= "Data")
	float BeamDuration = 2.5f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Data")
	TSubclassOf<UBaseDamageType> DamageType = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Beam VFX")
	UNiagaraSystem* BeamSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Beam VFX")
	UNiagaraSystem* BeamImpact = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Beam VFX")
	FName BeamStartName = FName("");

	UPROPERTY(EditDefaultsOnly, Category= "Beam VFX")
	FName BeamEndName = FName("");
};
