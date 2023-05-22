
#pragma once

#include "CoreMinimal.h"
#include "Components/Abilities/BaseAbility.h"
#include "BaseDodgeAbility.generated.h"

UCLASS()
class PROJECTELEMENT_API UBaseDodgeAbility : public UBaseAbility
{
	GENERATED_BODY()

	UBaseDodgeAbility();

	virtual void StartAbility() override;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Ability")
	UAnimSequence* DodgeForwardAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Ability")
	UAnimSequence* DodgeBackwardAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Ability")
	UAnimSequence* DodgeLeftAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Ability")
	UAnimSequence* DodgeRightAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Ability")
	UAnimSequence* DodgeInPlaceAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Ability")
	bool DodgeInPlace;

protected:
	void ChooseDodgeDirection();
	void PlayDodgeInPlace();
};
