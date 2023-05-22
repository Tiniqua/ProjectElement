
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraVisibilityComponent.generated.h"

class UTimelineComponent;
class UMaterialInstanceDynamic;
class UCurveFloat;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECTELEMENT_API UCameraVisibilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/** Constructor. */
	UCameraVisibilityComponent();

	/** Find subcomponents to hide when activated and create dynamic material instances to do so. */
	virtual void BeginPlay() override;

	/** Start to fade out meshes. */
	virtual void Activate(bool bReset) override;

	/** Start to fade in meshes that was opaque. */
	virtual void Deactivate() override;

	UFUNCTION()
	void FadeUpdate(float AlphaVal);
	
	UFUNCTION()
	void FadeFinished();

	/** Hide all meshes within this components owning actor depending if they are in-front of the camera view. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visibilty")
	bool bHideAllActorMeshes = true;

	/** List of object names of type skinned mesh within this component that will be hidden / shown depending if they are in-front of the camera view. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visibilty", meta = (EditCondition = "!bHideAllActorMeshes"))
	TArray<FName> ComponentNamesToHide;

	/** Min opacity to fade to. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visibilty")
	float MinOpacity = 0.0f;
	
	/** The time it takes to fade in or out. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visibilty|Timing", meta = (EditCondition = "FadeCurve == nullptr"))
	float TimeToFade = 1.2f;

	/** The curve to follow while fading, If null this fade will be linear. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visibilty|Timing")
	UCurveFloat* FadeCurve = nullptr;
	
protected:

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(Transient)
	UTimelineComponent* FadeTimeline = nullptr;

private:

	void InitialiseMaterials(const TArray<UActorComponent*>& Meshes);
};
