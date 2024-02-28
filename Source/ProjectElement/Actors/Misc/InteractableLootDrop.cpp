
#include "Actors/Misc/InteractableLootDrop.h"

#include "Actors/Character/ElementCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

class AElementCharacter;
// Sets default values
AInteractableLootDrop::AInteractableLootDrop()
{
	// Create a root component for the actor
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Create a static mesh component for the loot drop
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create a sphere component for collision
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(MeshComponent);

    // Set collision properties
    CollisionComponent->SetSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Interactable")); // Adjust collision profile if needed
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableLootDrop::OnBeginOverlap);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		
	// player and enemy channels
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
		
	CollisionComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AInteractableLootDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableLootDrop::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
										   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const AElementCharacter* ElementCharacter = Cast<AElementCharacter>(OtherActor))
	{
		if (APlayerController* PlayerController = ElementCharacter->GetController<APlayerController>())
		{
			if (AElementPlayerControllerBase* ElementPlayerController = Cast<AElementPlayerControllerBase>(PlayerController))
			{
				// TODO - Call the pickup function on the character
				//ElementCharacter->PickupLoot(this);

				// Destroy the loot drop
				Destroy();
			}
		}
	}
}

