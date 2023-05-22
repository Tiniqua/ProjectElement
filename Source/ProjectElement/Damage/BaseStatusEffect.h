
#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "BaseStatusEffect.generated.h"

class AElementCharacterBase;

UCLASS(Blueprintable, BlueprintType, Abstract)
class PROJECTELEMENT_API ABaseStatusEffect : public AActor
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(AElementCharacterBase* TargetCharacter);
	virtual void RemoveEffect(AElementCharacterBase* TargetCharacter);

	UPROPERTY()
	AActor* EffectCauser = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* StatusEffectVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	FName VFXStartupCharacterSocket;

protected:
	FTimerHandle EffectDurationHandle;
	FTimerHandle EffectTickHandle;

	UPROPERTY()
	UNiagaraComponent* NiagaraComponent = nullptr;
};
