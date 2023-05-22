
#include "Components/Abilities/AOEStrikeAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Actors/Character/ElementAiCharacter.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Damage/BaseDamageType.h"

void UAOEStrikeAbility::StartAbility()
{
	Super::StartAbility();
	
	if(OwnerCharacter == nullptr)
	{
		return;
	}

	CurrentRadius = DamageRadius;
	//Offset -= OwnerCharacter->GetActorRotation().Vector() * CurrentRadius;
	
	// start timer to create damage spheres
	GetWorld()->GetTimerManager().SetTimer(CreateSphereTimerHandle, this, &UAOEStrikeAbility::CreateDamageSphere, 0.1f, true);
}

void UAOEStrikeAbility::EndAbility()
{
	Super::EndAbility();
	
	GetWorld()->GetTimerManager().ClearTimer(CreateSphereTimerHandle);
	CurrentRadius = 0;
	Offset = FVector::ZeroVector;
	DamagedActors.Empty();
}

void UAOEStrikeAbility::CreateDamageSphere()
{
	if (CurrentRadius >= Range)
	{
		// reached range, stop timer
		GetWorld()->GetTimerManager().ClearTimer(CreateSphereTimerHandle);
		CurrentRadius = 0;
		Offset = FVector::ZeroVector;
		
		return;
	}

	USphereComponent* DamageSphere = NewObject<USphereComponent>(this);
	if (DamageSphere)
	{
		DamageSphere->SetSphereRadius(CurrentRadius);
		DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DamageSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		// player and enemy channels
		DamageSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		DamageSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);

		DamageSphere->SetGenerateOverlapEvents(true);
		DamageSphere->IgnoreActorWhenMoving(OwnerCharacter, true);
		DamageSphere->RegisterComponent();
		DamageSphere->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageSphere->AddRelativeLocation(Offset);
	}
	
	// calculate current radius and stop if range reached
	CurrentRadius += DamageRadius;

	const AElementAiCharacter* Owner = Cast<AElementAiCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	const AElementAIControllerBase* ControllerBase = Owner->GetElementAIController();
	if (!ControllerBase)
	{
		return;
	}

	const AActor* Target = Cast<AActor>(ControllerBase->CurrentCombatTarget);
	if (!Target)
	{
		return;
	}

	// Calculate the offset using the unit vector between the owner and the target character
	const FVector UnitDirection = (Target->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	Offset -= UnitDirection * CurrentRadius;
	
	//Offset -= OwnerCharacter->GetActorRotation().Vector() * CurrentRadius;
	
	if (CurrentRadius >= Range)
	{
		return;
	}

	// TODO - Move to debug menu as option behind CVAR
	if (ShouldDrawDebug)
	{
		DrawDebugSphere(GetWorld(),
			DamageSphere->GetComponentLocation(),
			CurrentRadius,
			12, FColor::Red,
			false,
			3.0f);
	}

	if(DamageVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			DamageVFX,
			DamageSphere->GetComponentLocation(),
			DamageSphere->GetComponentLocation().Rotation(),
			FVector(VFXScale)
			);
	}

	// damage overlapping actors
	TArray<AActor*> OverlappingActors;
	DamageSphere->GetOverlappingActors(OverlappingActors, AElementCharacterBase::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if(DamagedActors.Contains(Actor))
		{
			return;
		}
		
		const AElementCharacterBase* DamagedCharacter = Cast<AElementCharacterBase>(Actor);
		if (DamagedCharacter != nullptr)
		{
			UVitalityComponent* DamagedCharacterVitality = Cast<UVitalityComponent>(DamagedCharacter->HealthVitality);
			if (DamagedCharacterVitality != nullptr)
			{
				if (DamageType != nullptr)
				{
					const UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>(this, DamageType);
					if (DamageTypeInstance != nullptr)
					{
						DamagedCharacterVitality->ApplyDamage(GetOwner(), DamageTypeInstance);
						DamagedActors.Add(Actor);
					}
				}
			}
		}
	}

	// destroy damage spheres
	if (DamageSphere)
	{
		 DamageSphere->DestroyComponent();
	}
}
