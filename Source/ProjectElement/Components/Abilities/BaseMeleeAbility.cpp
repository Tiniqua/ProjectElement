
#include "Components/Abilities/BaseMeleeAbility.h"

#include "Actors/Character/ElementCharacter.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/BaseDamageType.h"
#include "Kismet/KismetSystemLibrary.h"

void UBaseMeleeAbility::StartAbility()
{
	Super::StartAbility();
	
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
		DamageSphere->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageSphere->AddRelativeLocation(Offset);
	}
	
	DamageSphere->GetOverlappingActors(OverlappingActors, AElementCharacterBase::StaticClass());

	// TODO - Move to debug menu as option behind CVAR
	if(ShouldDrawDebug)
	{
		DrawDebugSphere(GetWorld(),
		DamageSphere->GetComponentLocation(),
		DamageRadius,
		12, FColor::Red,
		false,
		3.0f);
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
}
