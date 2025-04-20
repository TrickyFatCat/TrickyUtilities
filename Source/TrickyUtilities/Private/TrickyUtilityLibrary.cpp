// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyUtilityLibrary.h"

#include "Components/TimelineComponent.h"

void UTrickyUtilityLibrary::CalculateTimelinePlayRate(UTimelineComponent* Timeline, const float DesiredTime)
{
	if (!IsValid(Timeline) || DesiredTime <= 0.f)
	{
		return;
	}

	const float TimelineLength = Timeline->GetTimelineLength();

	if (TimelineLength <= 0.f)
	{
		return;
	}

	const float PlayRate = TimelineLength / DesiredTime;
	Timeline->SetPlayRate(PlayRate);
}
