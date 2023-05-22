
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ElementCameraBoomArm.generated.h"

class UCameraVisibilityComponent;

/** Spring arm with added functionality in the collision test to activate / deactivate objects containing the
 *  UCameraVisibilityComponent to show / hide themselves when in-front of the camera. */
UCLASS()
class PROJECTELEMENT_API UElementCameraBoomArm : public USpringArmComponent
{
	GENERATED_BODY()

public:

	/** Should look for a UCameraVisibilityComponent that exists in their owning actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraCollision, meta=(editcondition="bDoCollisionTest"))
	bool bDoVisibilityCheck = true;
	
	/** Channel to look for any objects / meshes that may also have a UCameraVisibilityComponent that exists in their owning actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraCollision, meta=(editcondition="bDoCameraOverlapCheck && bDoCollisionTest"))
	TEnumAsByte<ECollisionChannel> CameraVisibilityCheck;
	
protected:
	
	/** Overriden to add functionality for looking for a UCameraVisibilityComponent on any object overlapping / blocking the camera. */
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;

	UPROPERTY()
	TArray<UCameraVisibilityComponent*> ActiveBlockCheckComponents;
};
