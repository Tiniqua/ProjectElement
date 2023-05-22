

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJECTELEMENT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

	virtual void InitializeComponent() override;

	// soft pointer references to ability classes to try and instantiate
	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftClassPtr<UActorComponent>> CharacterAbilities;

	// array of instanced and created abilities that have successfully been setup
	UPROPERTY()
	TArray<UBaseAbility*> InstancedAbilities;
	
	UFUNCTION()
	UBaseAbility* GetCurrentAbilityType(TSubclassOf<UBaseAbility> AbilityType) const;
		
};
