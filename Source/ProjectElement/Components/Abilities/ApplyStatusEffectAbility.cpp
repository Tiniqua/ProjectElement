
#include "Components/Abilities/ApplyStatusEffectAbility.h"

#include "Actors/Character/ElementCharacterBase.h"
#include "Damage/BaseStatusEffect.h"
#include "Kismet/GameplayStatics.h"

void UApplyStatusEffectAbility::StartAbility()
{
	Super::StartAbility();
	
	if(StatusEffectClass != nullptr)
	{
		TArray<AActor*> FoundActors;
		TArray<AElementCharacterBase*> CharactersInRadius;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElementCharacterBase::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			const float Distance = FVector::Dist(GetOwner()->GetActorLocation(), Actor->GetActorLocation());
			if (Distance <= RadiusToApply)
			{
				AElementCharacterBase* Character = Cast<AElementCharacterBase>(Actor);
				if (Character != nullptr && Actor != GetOwner())
				{
					CharactersInRadius.Add(Character);
				}
			}
		}

		for (AElementCharacterBase* Character : CharactersInRadius)
		{
			if (Character->HealthVitality->Faction == FactionToApplyTo)
			{
				// TODO :: Not using BP class reference
				ABaseStatusEffect* StatusEffect = NewObject<ABaseStatusEffect>(this, StatusEffectClass);
				StatusEffect->EffectCauser = OwnerCharacter;
				StatusEffect->ApplyEffect(Character);
			}
		}

		if (GetOwner() != nullptr && DrawDebug)
		{
			const FVector OwnerLocation = GetOwner()->GetActorLocation();
			DrawDebugSphere(GetWorld(), OwnerLocation, RadiusToApply, 12, FColor::Red, false, 3.0f, 0, 1.0f);
		}
	}
}
