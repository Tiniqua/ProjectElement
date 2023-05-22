
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

enum class EHitReactionType : uint8;
class AElementCharacterBase;
UENUM(BlueprintType)
enum class EFSMState : uint8
{
	Spawning,
	Common,
	Reaction,
	Death
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTELEMENT_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFSMComponent();

	UFUNCTION()
	virtual void HandleSpawningState();

	UFUNCTION()
	virtual void HandleCommonState();

	UFUNCTION()
	virtual void HandleReactionState(EHitReactionType HitReaction);

	UFUNCTION()
	virtual void HandleDeathState(AElementCharacterBase* InCharacterDied);

	virtual void SetState(uint8 NewState);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	uint8 CurrentState;
	
	// TODO Make pure virtual unless character needs FSM setup enemies dont use
	virtual void RunCurrentState();
};
