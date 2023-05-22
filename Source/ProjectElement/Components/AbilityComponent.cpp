
#include "Components/AbilityComponent.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bWantsInitializeComponent = true;
}

void UAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (const TSoftClassPtr<UActorComponent>& AbilityClass : CharacterAbilities)
	{
		// TODO: Someone will need to make an asset management system for startup to initialise assets asynchronously that will be used at any given time.
		const UClass* ClassRef = AbilityClass.LoadSynchronous();
		
		// Create the component on the owning actor 
		UActorComponent* AbilityInstance = NewObject<UActorComponent>(GetOwner(), ClassRef);
		check(AbilityInstance != nullptr);
	
		// Register activate, and initialize the component since the owner can't do it for us
		AbilityInstance->RegisterComponent();
	
		if (AbilityInstance->bAutoActivate)
		{
			AbilityInstance->Activate(true);
		}
	
		if (AbilityInstance->bWantsInitializeComponent)
		{
			AbilityInstance->InitializeComponent();
		}

		UBaseAbility* Ability = Cast<UBaseAbility>(AbilityInstance);
		InstancedAbilities.Add(Ability);
		
		if(Ability != nullptr)
		{
			Ability->SetupAbility();
		}
	}
}

UBaseAbility* UAbilityComponent::GetCurrentAbilityType(const TSubclassOf<UBaseAbility> AbilityType) const
{
	return Cast<UBaseAbility>(GetOwner()->GetComponentByClass(AbilityType));
}

