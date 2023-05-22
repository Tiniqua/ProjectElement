
#include "ElementCameraBoomArm.h"
#include "CameraVisibilityComponent.h"
#include "Actors/Character/ElementCharacter.h"
#include "Camera/CameraComponent.h"

void UElementCameraBoomArm::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);

	if (!bDoVisibilityCheck)
	{
		return;
	}
	
	const UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		return;
	}

	const AElementCharacter* FoundCharacter = Cast<AElementCharacter>(GetOwner());
	if (!FoundCharacter)
	{
		return;
	}

	const FVector StartTrace = GetComponentLocation();
	const FVector EndTrace = FoundCharacter->GetCamera()->GetComponentLocation();
	const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());
	TArray<FHitResult> Results;
	const bool bBlocked = CurrentWorld->SweepMultiByChannel(Results, StartTrace, EndTrace, FQuat::Identity, CameraVisibilityCheck, FCollisionShape::MakeSphere(ProbeSize), QueryParams);
	
	TArray<UCameraVisibilityComponent*> ResetBlockCheckComponents = ActiveBlockCheckComponents;
	if (Results.Num() > 0)
	{
		for (const FHitResult& Result : Results)
		{
			const AActor* FoundActor = Result.GetActor();
			if (!FoundActor)
			{
				continue;
			}

			UCameraVisibilityComponent* FoundBlockCheck = Cast<UCameraVisibilityComponent>(FoundActor->GetComponentByClass(UCameraVisibilityComponent::StaticClass()));
			if (!FoundBlockCheck)
			{
				continue;
			}

			ResetBlockCheckComponents.Remove(FoundBlockCheck);
			if (ActiveBlockCheckComponents.Contains(FoundBlockCheck))
			{
				continue;
			}
		
			FoundBlockCheck->Activate(false);
			ActiveBlockCheckComponents.Add(FoundBlockCheck);
		}
	}
	
	for (UCameraVisibilityComponent* BlockCheckComponent : ResetBlockCheckComponents)
	{
		if (!BlockCheckComponent)
		{
			continue;
		}

		BlockCheckComponent->Deactivate();
		ActiveBlockCheckComponents.Remove(BlockCheckComponent);
	}
}
