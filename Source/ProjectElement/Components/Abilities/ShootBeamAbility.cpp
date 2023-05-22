
#include "Components/Abilities/ShootBeamAbility.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Actors/Character/ElementAiCharacter.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Actors/Controllers/ElementAIControllerBase.h"
#include "Components/VitalityComponent.h"
#include "Damage/BaseDamageType.h"

UShootBeamAbility::UShootBeamAbility()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShootBeamAbility::StartAbility()
{
	Super::StartAbility();

	IsBeamActive = true;

	BeamCurrentEnd = GetOwner()->GetActorLocation();


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

	USkeletalMeshComponent* OwningMesh = Owner->GetMesh();
	if (!OwningMesh)
	{
		return;
	}
	
	if (BeamSystem)
	{
		BeamComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BeamSystem,
			OwningMesh,
			SocketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DealDamage, this, &UShootBeamAbility::TraceAndDealDamage, DamageInterval, true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeamDuration, this, &UShootBeamAbility::EndAbility, BeamDuration, false);
}

void UShootBeamAbility::EndAbility()
{
	Super::EndAbility();

	IsBeamActive = false;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DealDamage);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_BeamDuration);
}

void UShootBeamAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateBeam(DeltaTime);
}

void UShootBeamAbility::TraceAndDealDamage()
{
	if(IsBeamActive == false)
	{
		return;
	}
	
	// Define the trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("ShootBeamTrace")), true, GetOwner());
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	// Perform the line trace
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, BeamStart, BeamCurrentEnd, static_cast<ECollisionChannel>(ECC_GameTraceChannel2 | ECC_GameTraceChannel3), TraceParams);

	// If the line trace hit something, apply damage to the hit actor
	if (bHit)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (AElementCharacterBase* DamagedCharacter = Cast<AElementCharacterBase>(HitActor))
			{
				if (UVitalityComponent* DamagedCharacterVitality = Cast<UVitalityComponent>(DamagedCharacter->HealthVitality))
				{
					if (DamageType != nullptr)
					{
						if (const UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>(this, DamageType))
						{
							DamagedCharacterVitality->ApplyDamage(GetOwner(), DamageTypeInstance);
						}
					}
				}
			}
		}
	}
}

void UShootBeamAbility::UpdateBeam(float DeltaTime)
{
	if (!IsBeamActive)
	{
		return;
	}
    
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

	const USkeletalMeshComponent* OwningMesh = Owner->GetMesh();
	if (!OwningMesh)
	{
		return;
	}

	const FVector SocketLocation = OwningMesh->GetSocketLocation(SocketName);

	BeamStart = SocketLocation;
	BeamTargetEnd = Target->GetActorLocation();

	// Define the trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("ShootBeamTrace")), true, GetOwner());
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	// Perform the line trace
	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, BeamStart, BeamCurrentEnd, ECC_Visibility, TraceParams);

	// If the line trace hit something, apply damage to the hit actor
	if (bHit)
	{
		BeamCurrentEnd = HitResult.Location;
		
		if (BeamImpact)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				BeamImpact,
				HitResult.Location,
				HitResult.ImpactNormal.Rotation()
			);
		}
	}
	else
	{
		// Lerp the current beam end towards the target beam end
		BeamCurrentEnd = FMath::Lerp(BeamCurrentEnd, BeamTargetEnd, LerpSpeed * DeltaTime);
	}
	
	if (ShouldDrawDebug)
	{
		// Draw a debug line from BeamStart to BeamEnd
		DrawDebugLine(GetWorld(), BeamStart, BeamCurrentEnd, FColor::Red, false, -1.0f, 0, 5.0f);
	}

	if(BeamComponent)
	{
		BeamComponent->SetVectorParameter(BeamStartName, BeamStart);
		BeamComponent->SetVectorParameter(BeamEndName, BeamCurrentEnd);
	}
}