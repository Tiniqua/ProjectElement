
#include "Components/Abilities/AOETelegraphAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Damage/BaseDamageType.h"

void UAOETelegraphAbility::ActivateTelegraphedAbility()
{
	TArray<AActor*> OverlappingActors;

	USphereComponent* DamageSphere = NewObject<USphereComponent>(this);
	if (DamageSphere)
	{
		DamageSphere->SetSphereRadius(DamageRadius);
		DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DamageSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		// player and enemy channels
		DamageSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		DamageSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
		
		DamageSphere->SetGenerateOverlapEvents(true);
		DamageSphere->IgnoreActorWhenMoving(OwnerCharacter,true);
		DamageSphere->RegisterComponent();
		DamageSphere->SetWorldLocation(CurrentAbilityLocation);
	}

	DamageSphere->GetOverlappingActors(OverlappingActors, AElementCharacterBase::StaticClass());

	// TODO - Move to debug menu as option behind CVAR
	if(ShouldDrawDebug)
	{
		DrawDebugSphere(GetWorld(),
		CurrentAbilityLocation,
		DamageRadius,
		12, FColor::Green,
		false,
		3.0f);
	}

	if(DamageVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageVFX, CurrentAbilityLocation);
	}
	
	for (AActor* Actor : OverlappingActors)
	{
		const AElementCharacterBase* DamagedCharacter = Cast<AElementCharacterBase>(Actor);
		if(DamagedCharacter != nullptr)
		{
			UVitalityComponent* DamagedCharacterVitality = Cast<UVitalityComponent>(DamagedCharacter->HealthVitality);
			if(DamagedCharacterVitality != nullptr)
			{
				if(DamageType != nullptr)
				{
					const UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>(this, DamageType);
					if (DamageTypeInstance != nullptr)
					{
						DamagedCharacterVitality->ApplyDamage(GetOwner(), DamageTypeInstance);
					}
				}
			}
		}
	}
	DamageSphere->DestroyComponent();

	IsTelegraphPendingConfirm = false;
	Super::EndAbility();
}
