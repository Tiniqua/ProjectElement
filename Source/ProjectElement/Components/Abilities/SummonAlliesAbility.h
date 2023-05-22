
#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/Abilities/BaseAbility.h"
#include "SummonAlliesAbility.generated.h"

struct FEnvQueryResult;
class UEnvQuery;
class AElementAiCharacter;

UCLASS()
class PROJECTELEMENT_API USummonAlliesAbility : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void StartAbility() override;

	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summon")
	TSubclassOf<AElementAiCharacter> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summon")
	int32 EnemyCount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summon")
	UEnvQuery* SpawnLocationsQuery;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Summon")
	UNiagaraSystem* SummonLocationVFX = nullptr;
};
