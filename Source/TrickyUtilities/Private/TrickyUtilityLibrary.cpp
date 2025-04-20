// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyUtilityLibrary.h"

#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

void UTrickyUtilityLibrary::FormatTimeSeconds(const float TimeSeconds,
                                               const ETimeFormat TimeFormat,
                                               FString& OutResult)
{
	const FTimespan Timespan = UKismetMathLibrary::FromSeconds(TimeSeconds);
	const int32 TotalMinutes = static_cast<int32>(Timespan.GetTotalMinutes());
	const int32 Seconds = Timespan.GetSeconds();
	const int32 TotalSeconds = (FMath::CeilToInt(Timespan.GetTotalSeconds()));
	const int32 Milliseconds = Timespan.GetFractionMilli();

	auto ConvertMilliseconds = [&Milliseconds](const float Fraction) -> int32
	{
		return static_cast<int32>(Milliseconds * Fraction);
	};

	switch (TimeFormat)
	{
	case ETimeFormat::MM_SS_MsMs:
		OutResult = FString::Printf(TEXT("%02d:%02d.%02d"),
		                         TotalMinutes,
		                         Seconds,
		                         ConvertMilliseconds(0.1f));
		break;

	case ETimeFormat::MM_SS_Ms:
		OutResult = FString::Printf(TEXT("%02d:%02d.%d"),
		                         TotalMinutes,
		                         Seconds,
		                         ConvertMilliseconds(0.01f));
		break;

	case ETimeFormat::MM_SS:
		OutResult = FString::Printf(TEXT("%02d:%02d"), TotalMinutes, Seconds);
		break;

	case ETimeFormat::SS_MsMs:
		OutResult = FString::Printf(TEXT("%02d.%02d"), TotalSeconds, ConvertMilliseconds(0.1f));
		break;

	case ETimeFormat::SS_Ms:
		OutResult = FString::Printf(TEXT("%02d.%d"), TotalSeconds, ConvertMilliseconds(0.01f));
		break;

	case ETimeFormat::SS:
		OutResult = FString::Printf(TEXT("%02d"), TotalSeconds);
		break;
	}
}
