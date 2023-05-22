

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "BaseAbility.generated.h"


class UNiagaraSystem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTELEMENT_API UBaseAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* StartupMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	bool UpperBodyAnim = false;

	UPROPERTY(EditDefaultsOnly, Category = "Tag")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* AbilityStartupVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	FName VFXStartupCharacterSocket;

	UPROPERTY()
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	// Handle initialisation 
	virtual void SetupAbility();

	// Called by BTTask for AI
	// TODO Player method for calling bound to input system
	virtual void PlayStartupMontage();
	
	UFUNCTION()
	virtual void StartAbility();

	void ResetCooldown();
	
	UFUNCTION()
	virtual void ActivateTelegraphedAbility();

	UFUNCTION()
	virtual void EndAbility();

	UFUNCTION()
	void StartAbilityIfTagsMatch(FGameplayTag TriggeredAbilityTag);
	
	void ResetAnimState();
	bool GetIsPlayingAnimation() const {return IsPlayingAnimation;};

	FTimerHandle AbilityMontage;
	bool IsPlayingAnimation = false;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
	float CooldownDuration = 1.0f;

	UPROPERTY()
	bool IsAbilityOnCooldown = false;

	UPROPERTY(EditDefaultsOnly, Category = "Telegraph")
	bool IsTelegraphedAbility = false;

	UPROPERTY()
	bool IsTelegraphPendingConfirm = false;

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;
};
