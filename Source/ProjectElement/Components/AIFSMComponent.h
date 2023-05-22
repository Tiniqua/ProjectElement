
#pragma once

#include "CoreMinimal.h"
#include "Components/FSMComponent.h"
#include "AIFSMComponent.generated.h"

enum class EHitReactionType : uint8;
UCLASS()
class PROJECTELEMENT_API UAIFSMComponent : public UFSMComponent
{
	GENERATED_BODY()
	
	virtual void RunCurrentState() override;
	
	virtual void HandleSpawningState() override;
	
	virtual void HandleCommonState() override;

	virtual void HandleDeathState(AElementCharacterBase* InCharacterDied) override;

	void OnSpawnAnimComplete();
	void OnDeathAnimComplete();
	void OnReactionAnimComplete();
	
	FTimerHandle SpawnAnimTimer;
	FTimerHandle ReactionAnimTimer;
	FTimerHandle DeathAnimTimer;

public:
	UPROPERTY()
	UAnimMontage* ReactionAnimation = nullptr;

	virtual void HandleReactionState(EHitReactionType HitReaction) override;
};
