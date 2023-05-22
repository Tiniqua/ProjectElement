
#include "Damage/BaseProjectile.h"

#include "BaseDamageType.h"
#include "NiagaraComponentPool.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(16.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = RootComponent;
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);	
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	const UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return;
	}
	
	
	// TODO - Consider faction interactions / should enemies be able to damage enemies
	if(const AElementCharacterBase* HitCharacter = Cast<AElementCharacterBase>(OtherActor))
	{
		UVitalityComponent* DamagedCharacterVitality = Cast<UVitalityComponent>(HitCharacter->HealthVitality);
		if(DamagedCharacterVitality != nullptr)
		{
			if(DamageType != nullptr)
			{
				const UBaseDamageType* DamageTypeInstance = NewObject<UBaseDamageType>(this, DamageType);
				if (DamageTypeInstance != nullptr)
				{
					if(SourceActor)
					{
						DamagedCharacterVitality->ApplyDamage(SourceActor, DamageTypeInstance);
					}
				}
			}
		}
	}
	
	if(ImpactVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ImpactVFX, Hit.Location);
	}

	// TEMP DISABLED - need to look into further with mesh and capsule collisions
	// if(const AElementCharacterBase* Character = Cast<AElementCharacterBase>(OtherActor))
	// {
	// 	USceneComponent* CharacterMesh = Character->GetMesh();
	// 	if (CharacterMesh && MeshComponent && ShouldAttachOnImpact)
	// 	{
	// 		MeshComponent->AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
	// 		MeshComponent->SetSimulatePhysics(false);
	//
	// 		// Set a timer to detach the projectile mesh after the specified duration
	// 		World->GetTimerManager().SetTimer(WeldTimerHandle, this, &ABaseProjectile::DetachProjectile, AttachLifeSpan, false);
	// 	}
	// }
	
	Destroy();
}

void ABaseProjectile::DetachProjectile()
{
	if (MeshComponent)
	{
		MeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		MeshComponent->SetSimulatePhysics(true);
		Destroy();

		if(const UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(WeldTimerHandle);
		}
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

