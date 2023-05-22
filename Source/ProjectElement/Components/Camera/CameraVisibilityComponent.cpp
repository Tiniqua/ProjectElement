
#include "CameraVisibilityComponent.h"
#include "Library/ElementFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraVisibility, Log, All);

UCameraVisibilityComponent::UCameraVisibilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
}

void UCameraVisibilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* OwningActor = GetOwner();
	if (!OwningActor)
	{
		return;
	}

	// Setup timeline.
	const FName TimelineName = MakeUniqueObjectName(this, UTimelineComponent::StaticClass(), "FadeTimeline");
	FTimelineDelegate TimelineDel;
	TimelineDel.BindTimelineUFunctions(this, "FadeUpdate", "FadeFinished");
	if (FadeCurve)
	{
		FadeTimeline = UElementFunctionLibrary::CreateTimelineFromCurve(TimelineDel, FadeCurve, TimelineName, OwningActor);
	}
	else
	{
		FadeTimeline = UElementFunctionLibrary::CreateLinearTimeline(TimelineDel, TimeToFade, TimelineName, OwningActor);
	}
	
	// Find all dynamic materials that will change during runtime.
	if (bHideAllActorMeshes)
	{
		TArray<UActorComponent*> FoundComponents;
		OwningActor->GetComponents(UMeshComponent::StaticClass(),FoundComponents);
		if (FoundComponents.Num() <= 0)
		{
			return;
		}

		InitialiseMaterials(FoundComponents);
		return;
	}

	TArray<UActorComponent*> FoundComponents;
	for(UActorComponent* Comp : OwningActor->GetComponents())
	{
		if (!Comp)
		{
			continue;;
		}
		
		if (ComponentNamesToHide.Contains(Comp->GetFName()))
		{
			if (!FoundComponents.Contains(Comp))
			{
				continue;
			}
			
			FoundComponents.Add(Comp);
		}
	}

	InitialiseMaterials(FoundComponents);
}

void UCameraVisibilityComponent::InitialiseMaterials(const TArray<UActorComponent*>& Meshes)
{
	for (UActorComponent* FoundMesh : Meshes)
	{
		UMeshComponent* Mesh = Cast<UMeshComponent>(FoundMesh);
		if (!Mesh)
		{
			continue;
		}

		for (int m = 0; m < Mesh->GetNumMaterials(); m++)
		{
			UMaterialInstanceDynamic* NewMaterialInstance = Mesh->CreateDynamicMaterialInstance(m, Mesh->GetMaterial(m));
			Mesh->SetMaterial(m, NewMaterialInstance);
			DynamicMaterials.Add(NewMaterialInstance);
		}
	}
}

void UCameraVisibilityComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	if (DynamicMaterials.Num() <= 0)
	{
		UE_LOG(LogCameraVisibility, Warning, TEXT("%s: Trying to activate UCameraVisibilityComponent %s with empty list of DynamicMaterials?!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}
	
	if (!FadeTimeline)
	{
		UE_LOG(LogCameraVisibility, Warning, TEXT("%s: Trying to activate UCameraVisibilityComponent %s with empty FadeTimeline?!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}
	
	// Fade in.
	FadeTimeline->Play();
}

void UCameraVisibilityComponent::Deactivate()
{
	Super::Deactivate();

	if (DynamicMaterials.Num() <= 0)
	{
		UE_LOG(LogCameraVisibility, Warning, TEXT("%s: Trying to deactivate UCameraVisibilityComponent %s with empty list of DynamicMaterials?!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}
	
	if (!FadeTimeline)
	{
		UE_LOG(LogCameraVisibility, Warning, TEXT("%s: Trying to deactivate UCameraVisibilityComponent %s with empty FadeTimeline?!"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}
	
	// Fade out.
	FadeTimeline->Reverse();
}

void UCameraVisibilityComponent::FadeUpdate(float AlphaVal)
{
	const float InverseAlpha = 1.0f - AlphaVal;
	const float MappedAlpha = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(MinOpacity, 1.0f), InverseAlpha);
	for (UMaterialInstanceDynamic* Material : DynamicMaterials)
	{
		if (!Material)
		{
			continue;
		}
		
		Material->SetScalarParameterValue("Opacity", MappedAlpha);
	}
}

void UCameraVisibilityComponent::FadeFinished()
{
	//...
}