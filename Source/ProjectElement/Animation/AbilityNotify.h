
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AbilityNotify.generated.h"

UCLASS()
class PROJECTELEMENT_API UAbilityNotify : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// Tag for matching ability to notify
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag NotifyTag = FGameplayTag::EmptyTag;
};
