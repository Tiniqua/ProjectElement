
#include "ElementFunctionLibrary.h"

UTimelineComponent* UElementFunctionLibrary::CreateTimelineFromCurve(FTimelineDelegate InTimelineDel,
	UCurveFloat* InTimelineCurve, FName InTimelineName, AActor* InOwner, bool bLooping,
	ETimelineLengthMode InTimelineLengthMode)
{
	if (!InOwner)
	{
		return nullptr;
	}
	
	UTimelineComponent* NewTimeline = NewObject<UTimelineComponent>(InOwner, InTimelineName);
	NewTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	InOwner->BlueprintCreatedComponents.Add(NewTimeline);

	NewTimeline->SetPropertySetObject(InTimelineDel.PropertySetObject);
	NewTimeline->SetLooping(bLooping);
	NewTimeline->SetTimelineLengthMode(InTimelineLengthMode);
	NewTimeline->SetPlaybackPosition(0.0f, false, false);
	NewTimeline->SetTimelineFinishedFunc(InTimelineDel.FinishedDel);
	NewTimeline->AddInterpFloat(InTimelineCurve, InTimelineDel.UpdateDel, FName("Alpha"));
	NewTimeline->RegisterComponent();
	return NewTimeline;
}

UTimelineComponent* UElementFunctionLibrary::CreateLinearTimeline(FTimelineDelegate InTimelineDel, float InTime,
	FName InTimelineName, AActor* InOwner, bool bLooping, ETimelineLengthMode InTimelineLengthMode)
{
	FRichCurve* NewFloatCurve = new FRichCurve();
	NewFloatCurve->SetKeyInterpMode(NewFloatCurve->AddKey(0.0f, 0.0f), ERichCurveInterpMode::RCIM_Linear);
	NewFloatCurve->SetKeyInterpMode(NewFloatCurve->AddKey(InTime, 1.0f), ERichCurveInterpMode::RCIM_Linear);

	UCurveFloat* NewTimelineCurve = NewObject<UCurveFloat>();
	NewTimelineCurve->FloatCurve = *NewFloatCurve;
	delete NewFloatCurve;
	
	return CreateTimelineFromCurve(InTimelineDel, NewTimelineCurve, InTimelineName, InOwner, bLooping, InTimelineLengthMode);
}
