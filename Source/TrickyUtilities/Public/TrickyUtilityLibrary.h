// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TrickyUtilityLibrary.generated.h"

class UTimelineComponent;

UENUM(BlueprintType)
enum class ETimeFormat : uint8
{
	MM_SS_MsMs UMETA(DisplayName="MM:SS.MsMs"),
	MM_SS_Ms UMETA(DisplayName="MM:SS.Ms"),
	MM_SS UMETA(DisplayName="MM:SS"),
	SS_MsMs UMETA(DisplayName="SS.MsMs"),
	SS_Ms UMETA(DisplayName="SS.Ms"),
	SS UMETA(DisplayName="SS")
};

UENUM()
enum class EPointDirection : uint8
{
	Origin UMETA(DisplayName="Origin"),
	Clockwise UMETA(DisplayName="Clockwise"),
	CounterClockwise UMETA(DisplayName="CounterClockwise"),
	Inside UMETA(DisplayName="Inside"),
	Outside UMETA(DisplayName="Outside")
};

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
	 * @param Timeline A pointer to the UTimelineComponent whose play rate will be adjusted. Must be a valid instance.
	 * @param DesiredTime The desired duration in seconds for the timeline playback. Must be greater than 0.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateTimelinePlayRate(UTimelineComponent* Timeline, const float DesiredTime);

	/**
	 * Formats a given time duration in seconds into a string in the specified format.
	 *
	 * @param TimeSeconds The time in seconds that needs to be formatted. Must be greater than 0.
	 * @param TimeFormat The desired format.
	 * @param OutResult A reference to a string where the formatted time will be stored.
	 */
	UFUNCTION(BlueprintPure, Category="TrickyUtilities")
	static void FormatTimeSeconds(const float TimeSeconds, const ETimeFormat TimeFormat, FString& OutResult);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities", meta=(AutoCreateRefTerm="CellSize"))
	static void CalculateGridTransforms(const FTransform& Origin,
	                                    const int32 SizeX,
	                                    const int32 SizeY,
	                                    const FVector2D& CellSize,
	                                    TArray<FTransform>& OutTransforms);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities", meta=(AutoCreateRefTerm="CellSize"))
	static void CalculateCubeTransforms(const FTransform& Origin,
	                                    const int32 SizeX,
	                                    const int32 SizeY,
	                                    const int32 SizeZ,
	                                    const FVector& CellSize,
	                                    TArray<FTransform>& OutTransforms);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateRingTransform(const FTransform& Origin,
	                                   const int32 PointsAmount,
	                                   const float Radius,
	                                   const EPointDirection Direction,
	                                   TArray<FTransform>& OutTransforms);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateCylinderTransforms(const FTransform& Origin,
	                                        const int32 RingsAmount,
	                                        const int32 PointsAmount,
	                                        const float Radius,
	                                        const float Height,
	                                        const float RingDeltaAngle,
	                                        const EPointDirection Direction,
	                                        TArray<FTransform>& OutTransforms);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateArcTransforms(const FTransform& Origin,
	                                   const int32 PointsAmount,
	                                   const float Radius,
	                                   const float AngleDeg,
	                                   const EPointDirection Direction,
	                                   TArray<FTransform>& OutTransforms);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateArcCylinderTransforms(const FTransform& Origin,
	                                           const int32 ArcsAmount,
	                                           const int32 PointsAmount,
	                                           const float Radius,
	                                           const float Height,
	                                           const float AngleDeg,
	                                           const EPointDirection Direction,
	                                           TArray<FTransform>& OutTransforms);

	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateSphereTransforms(const FTransform& Origin,
	                                      const float Radius,
	                                      const int32 PointsAmount,
	                                      const float MinLatitude,
	                                      const float MaxLatitude,
	                                      const float MinLongitude,
	                                      const float MaxLongitude,
	                                      const EPointDirection Direction,
	                                      TArray<FTransform>& OutTransforms);
};
