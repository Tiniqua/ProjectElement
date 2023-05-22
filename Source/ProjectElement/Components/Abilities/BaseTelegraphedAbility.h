
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseAbility.h"
#include "BaseTelegraphedAbility.generated.h"

class UNiagaraComponent;
UCLASS()
class PROJECTELEMENT_API UBaseTelegraphedAbility : public UBaseAbility
{
	GENERATED_BODY()
	
	virtual void StartAbility() override;

	void CreateTelegraph(float DeltaTime);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ActivateTelegraphedAbility() override;

public:
	UBaseTelegraphedAbility();
	
	UPROPERTY(EditDefaultsOnly, Category= "Debug")
	bool ShouldDrawDebug = true;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* TelegraphVFX = nullptr;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent = nullptr;
	
	UPROPERTY()
	FVector CurrentAbilityLocation = FVector::ZeroVector;

	virtual void EndAbility() override;
};
