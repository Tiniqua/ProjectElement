

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UBaseDamageType;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PROJECTELEMENT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY()
	AActor* SourceActor = nullptr;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UBaseDamageType> DamageType = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UNiagaraSystem* ImpactVFX = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	bool ShouldAttachOnImpact = false;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AttachLifeSpan = 0.0f;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void DetachProjectile();


public:	
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle WeldTimerHandle;

};
