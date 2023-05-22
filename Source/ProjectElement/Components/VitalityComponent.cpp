// Copyright Epic Games, Inc. All Rights Reserved.

#include "VitalityComponent.h"

#include "Actors/Character/ElementAiCharacter.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Damage/BaseDamageType.h"

DEFINE_LOG_CATEGORY(LogVitalityComponent)

UVitalityComponent::UVitalityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVitalityComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmount = MaxAmount;
	RegenDelegate.BindUFunction(this, "RegenEvent");
}

bool UVitalityComponent::ApplyDamage(AActor* InDamageSource, const UBaseDamageType* DamageType)
{
	const UVitalityComponent* SourceVitality = InDamageSource->FindComponentByClass<UVitalityComponent>();
	if (SourceVitality && SourceVitality->Faction == Faction)
	{
		// The damage source and receiver are of the same faction, so do not apply damage
		// unless damage is negative and acting as healing
		if(DamageType->DamageAmount >= 0)
		{
			return false;
		}
	}
	
	if(DamageType != nullptr)
	{
		if(DamageType->StatusEffectClass != nullptr)
		{
			if(InDamageSource != nullptr)
			{
				ABaseStatusEffect* StatusEffect = NewObject<ABaseStatusEffect>(this, DamageType->StatusEffectClass);
				AElementCharacterBase* Character = Cast<AElementCharacterBase>(GetOwner());
				StatusEffect->EffectCauser = InDamageSource;
				StatusEffect->ApplyEffect(Character);
			}
		}

		if(DamageType->HitReactionInfo != EHitReactionType::None)
		{
			// TODO - expand to players too
			if(const AElementAiCharacter* AiCharacter = Cast<AElementAiCharacter>(GetOwner()))
			{
				if(AiCharacter->AIFSMComponent)
				{
					AiCharacter->AIFSMComponent->HandleReactionState(DamageType->HitReactionInfo);
				}
			}
		}
		
		const float InDamageAmount = DamageType->DamageAmount;

		if (InDamageAmount == 0.0f)
		{
			return false;
		}
	
		if (CurrentAmount == 0)
		{
			if (!bMarkedDepleted)
			{
				MarkVitalityDepleted();
			}
			return true;
		}

		if (CurrentAmount - InDamageAmount <= 0.0f)
		{
			MarkVitalityDepleted();
			DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), TEXT("Killed!"), nullptr, FColor::Red, 1.0f, false,2.f);
			return true;
		}

		CurrentAmount -= InDamageAmount;
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), FString::Printf(TEXT("Damage Taken: %i"),  FMath::RoundToInt(InDamageAmount)), nullptr, FColor::Red, 1.0f, false);
		
		OnReceivedDamageEvent.Broadcast(InDamageAmount, InDamageSource);

		if (GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
		}

		FTimerDelegate StartRegenDel;
		StartRegenDel.BindUFunction(this, "StartRegen", false);
		GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, RegenDelegate, RegenCooldown, false, RegenCooldown);
		return false;
	}

	UE_LOG(LogVitalityComponent, Warning, TEXT("%s: Character %s cannot receive damage because the DamageType is null!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
	return false;
}

void UVitalityComponent::ResetVitality()
{
	CurrentAmount = MaxAmount;
	OnVitalityResetEvent.Broadcast();
	OnVitalityRegenEvent.Broadcast(MaxAmount, 1.0f);
	
	if (GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
}

void UVitalityComponent::StartRegen(bool bStartFromDepleted)
{
	if (!bStartFromDepleted && bMarkedDepleted)
	{
		return;
	}
	
	if (GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
	GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, RegenDelegate, RegenRate, true, 0.0f);
}

void UVitalityComponent::RegenEvent()
{
	CurrentAmount = FMath::Clamp(CurrentAmount + RegenAmount, 0.0f, MaxAmount);
	if (CurrentAmount == MaxAmount)
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}
	
	OnVitalityRegenEvent.Broadcast(CurrentAmount, CurrentAmount / MaxAmount);
}

void UVitalityComponent::MarkVitalityDepleted()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(RegenTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
	}

	CurrentAmount = 0.0f;
	bMarkedDepleted = true;
	OnVitalityDepletedEvent.Broadcast();
}

float UVitalityComponent::GetCurrentVitalityAlpha() const
{
	if (CurrentAmount == 0.0f)
	{
		return 0.0f;
	}
	
	return CurrentAmount / MaxAmount;
}
