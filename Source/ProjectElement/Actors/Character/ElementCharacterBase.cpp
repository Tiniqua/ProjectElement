
#include "ElementCharacterBase.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Damage/BaseDamageType.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Modes/ElementBaseGameMode.h"

DEFINE_LOG_CATEGORY(LogElementCharacter)

AElementCharacterBase::AElementCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthVitality = CreateDefaultSubobject<UVitalityComponent>(TEXT("HealthVitality"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
}

void AElementCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterCapsule = GetCapsuleComponent();
	if (!CharacterCapsule)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: Character %s has no capsule?"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
	}

	CharacterMesh = GetMesh();
	if (!CharacterMesh)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: Character %s has no mesh?"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
	}
}

void AElementCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: Spawned a character with no valid world?! %s"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}
	
	GameMode = Cast<AElementBaseGameMode>(CurrentWorld->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: Character: %s cannot find the AElementBaseGameMode, some things will not function correctly!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
	}

	HealthVitality->OnVitalityDepletedEvent.AddDynamic(this, &AElementCharacterBase::OnDeathInternal);
	HealthVitality->OnVitalityRegenEvent.AddDynamic(this, &AElementCharacterBase::OnHealthRegenInternal);
}

void AElementCharacterBase::FellOutOfWorld(const UDamageType& dmgType)
{
	UNavigationSystemV1* NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (NavigationSystem)
	{
		FVector FoundLocation = FVector(0);
		const FVector SearchExtent = FVector(10000.0f);
		TArray<FNavDataConfig> NavigationProperties = NavigationSystem->GetSupportedAgents();
		FNavAgentProperties PropertiesToUse = NavigationProperties[0];
		if (UPawnMovementComponent* PawnMovement = GetMovementComponent())
		{
			PropertiesToUse = PawnMovement->GetNavAgentPropertiesRef();
		}

		ANavigationData* NavigationData = NavigationSystem->GetNavDataForProps(PropertiesToUse);
		if (NavigationSystem->K2_ProjectPointToNavigation(GetWorld(), GetActorLocation(), FoundLocation, NavigationData, nullptr, SearchExtent))
		{
			SetActorLocation(FoundLocation, false, nullptr, ETeleportType::ResetPhysics);
			return;
		}
	}
	
	if (!RestartPlayerRuntime())
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: Failed to restart player on navmesh and to RestartPlayerRuntime, destroying instead."), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		Super::FellOutOfWorld(dmgType);
	}
}

bool AElementCharacterBase::RestartPlayerRuntime()
{
	const UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogElementCharacter, Error, TEXT("%s: No valid world to retreive player starts?! %s"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}

	APlayerStart* FoundStart = nullptr;
	for (TActorIterator<APlayerStart> It(CurrentWorld, APlayerStart::StaticClass()); It; ++It)
	{
		FoundStart = *It;
		if (!FoundStart)
		{
			continue;
		}

		break;
	}

	if (!FoundStart)
	{
		return false;
	}

	SetActorLocationAndRotation(FoundStart->GetActorLocation(), FoundStart->GetActorRotation(), false, nullptr, ETeleportType::ResetPhysics);
	return true;
}

void AElementCharacterBase::KillCharacter()
{
	KillCharacterInternal();
}

void AElementCharacterBase::KillCharacterInternal()
{
	if (!HealthVitality)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: Character %s cannot apply death to health vitality because it is null!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		OnDeath();
		return;
	}
	
	UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>();
	DamageTypeInstance->DamageAmount = HealthVitality->MaxAmount;
	
	DamageCharacter(DamageTypeInstance, this);
}

bool AElementCharacterBase::Damage_Implementation(const UBaseDamageType* DamageType, AActor* InDamageSource)
{	
	return DamageCharacter(DamageType, InDamageSource);
}

bool AElementCharacterBase::K2_DamageCharacter(const UBaseDamageType* DamageType, AActor* InDamageSource)
{	
	return DamageCharacter(DamageType, InDamageSource);
}

bool AElementCharacterBase::DamageCharacter(const UBaseDamageType* DamageType, AActor* InDamageSource)
{
	if (bDead)
	{
		return true;
	}
	
	if (!HealthVitality)
	{
		UE_LOG(LogElementCharacter, Warning, TEXT("%s: Character %s cannot receive damage because the HealthVitality is null!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return false;
	}
	
	return HealthVitality->ApplyDamage(InDamageSource, DamageType);
}

void AElementCharacterBase::OnDeathInternal()
{
	OnDeath();
}

void AElementCharacterBase::OnDeath()
{
	// Maybe could pass final striking actor through here?
	bDead = true;
	OnCharacterDeathEvent.Broadcast(this);
	UE_LOG(LogElementCharacter, Log, TEXT("%s: Character %s has died!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
}

void AElementCharacterBase::OnHealthRegenInternal(float InNewVitality, float InNewVitalityAlpha)
{
	OnHealthRegen(InNewVitality, InNewVitalityAlpha);
}

void AElementCharacterBase::OnAbilityModuleNotify(const FGameplayTag NotifyTag) const
{
	// TODO - Only call broadcast if given tag matches current ability tag

	if(AbilityComponent)
	{
		for (const UBaseAbility* Ability : AbilityComponent->InstancedAbilities)
		{
			if (Ability->AbilityTag == NotifyTag)
			{
				OnCharacterAbilityEventStarted.Broadcast(NotifyTag);
				return;
			}
		}
	}

	UE_LOG(LogElementCharacter, Warning, TEXT("%s: Character %s cannot find any ability with required tag!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
}
