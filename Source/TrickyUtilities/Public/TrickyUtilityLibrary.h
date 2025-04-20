// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TrickyUtilityLibrary.generated.h"

class UTimelineComponent;
/**
 * 
 */
UCLASS()
class TRICKYUTILITIES_API UTrickyUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Adjusts the play rate of a specified UTimelineComponent to achieve the desired playback duration.
	 *
	 * The method calculates the play rate based on the timeline's total length and the desired time,
	 * then assigns the calculated rate to the timeline. If the input timeline is invalid,
	 * or if the desired time or timeline length is zero or negative, the function does nothing.
	 *
	 * @param Timeline A pointer to the UTimelineComponent whose play rate will be adjusted.
	 *                 Must be a valid instance.
	 * @param DesiredTime The desired duration in seconds for the timeline playback.
	 *                    Must be greater than 0.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateTimelinePlayRate(UTimelineComponent* Timeline, const float DesiredTime);
};
