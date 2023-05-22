
#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Components/TimelineComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ElementFunctionLibrary.generated.h"

/** Quick timeline delegate function assignment struct. */
USTRUCT()
struct FTimelineDelegate
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	UObject* PropertySetObject = nullptr;
	
	FOnTimelineFloat UpdateDel;
	FOnTimelineEventStatic FinishedDel;

	void BindTimelineUFunctions(UObject* InObject, const FName& InUpdateFunctionName, const FName& InFinishedFunctionName)
	{
		if (!InObject)
		{
			return;
		}
		
		PropertySetObject = InObject;
		UpdateDel.BindUFunction(InObject, InUpdateFunctionName);
		FinishedDel.BindUFunction(InObject, InFinishedFunctionName);
	}
};

UCLASS(meta=(ScriptName="ElementFunctionLibrary"))
class PROJECTELEMENT_API UElementFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	 /** Return a time line and setup from a given curve and FTimelineDelegate.
	 * @Param InTimelineDel, The function assignment for the timeline update and finished events.
	 * @Param InTimelineCurve, The curve to use for the timeline.
	 * @Param InTimelineName, The name of the time line.
	 * @Param propertySetObject, 
	 * @Param InOwner, The actor that this time line will be owned by.
	 * @Param bLooping, Should the timeline continue to loop after playing the last key frame.
	 * @Param InTimelineLengthMode, When should this timeline class as finished.*/
	static UTimelineComponent* CreateTimelineFromCurve(FTimelineDelegate InTimelineDel, UCurveFloat* InTimelineCurve, FName InTimelineName, AActor* InOwner, bool bLooping = false,
			ETimelineLengthMode InTimelineLengthMode = ETimelineLengthMode::TL_LastKeyFrame);


	/** Return a time line and setup from a given time/length and FTimelineDelegate.
	* @Param InTimelineDel, The function assignment for the timeline update and finished events.
	* @Param InTime, The time the timeline will last.
	* @Param InTimelineName, The name of the time line.
	* @Param propertySetObject, 
	* @Param InOwner, The actor that this time line will be owned by.
	* @Param bLooping, Should the timeline continue to loop after playing the last key frame.
	* @Param InTimelineLengthMode, When should this timeline class as finished. */
	static UTimelineComponent* CreateLinearTimeline(FTimelineDelegate InTimelineDel, float InTime, FName InTimelineName, AActor* InOwner, bool bLooping = false,
			ETimelineLengthMode InTimelineLengthMode = ETimelineLengthMode::TL_LastKeyFrame);
};
