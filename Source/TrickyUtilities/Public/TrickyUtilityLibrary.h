// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TrickyUtilityLibrary.generated.h"

class USplineComponent;
class UTimelineComponent;

UENUM(BlueprintType)
enum class EStringTimeFormat : uint8
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
	 * Calculates and sets a play rate of a given UTimeLine component by using its length and given time.
	 *
	 * @param Timeline A pointer to the UTimelineComponent. Must be a valid instance.
	 * @param DesiredTime The desired duration in seconds. Must be greater than 0.
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
	static void FormatTimeSeconds(const float TimeSeconds, const EStringTimeFormat TimeFormat, FString& OutResult);

	/**
	 * Calculates a grid of transforms based on an origin transform, grid size, and cell size.
	 *
	 * @param Origin The transform used as the starting point and reference coordinate for the grid.
	 * @param SizeX The number of cells along the X-axis. Must be greater than 0.
	 * @param SizeY The number of cells along the Y-axis. Must be greater than 0.
	 * @param CellSize Defines the size of each cell in the grid along the X and Y axes. Values must be greater than 0.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities", meta=(AutoCreateRefTerm="CellSize"))
	static void CalculateGridTransforms(const FTransform& Origin,
	                                    const int32 SizeX,
	                                    const int32 SizeY,
	                                    const FVector2D& CellSize,
	                                    TArray<FTransform>& OutTransforms);

	/**
	 * Calculates a series of transforms that form a 3D grid (cube) based on the specified origin, size, and cell dimensions.
	 *
	 * @param Origin The transform that serves as the origin and basis for all calculated cell transforms.
	 * @param SizeX The number of cells along the X-axis. Must be greater than 0.
	 * @param SizeY The number of cells along the Y-axis. Must be greater than 0.
	 * @param SizeZ The number of cells along the Z-axis. Must be greater than 0.
	 * @param CellSize The dimensions of each cell.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities", meta=(AutoCreateRefTerm="CellSize"))
	static void CalculateCubeTransforms(const FTransform& Origin,
	                                    const int32 SizeX,
	                                    const int32 SizeY,
	                                    const int32 SizeZ,
	                                    const FVector& CellSize,
	                                    TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms arranged in a circular pattern around a given origin.
	 *
	 * @param Origin Centre of the ring.
	 * @param PointsAmount The number of points in the ring. Must be greater than 0.
	 * @param Radius The radius of the ring. Must be greater than 0.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateRingTransform(const FTransform& Origin,
	                                   const int32 PointsAmount,
	                                   const float Radius,
	                                   const EPointDirection Direction,
	                                   TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms for points arranged in multiple concentric rings around a given origin.
	 *
	 * @param Origin Centre of the pattern.
	 * @param RingsAmount The number of concentric rings to generate. Must be greater than 0.
	 * @param PointsPerRing The number of points in each ring. Must be greater than 0.
	 * @param MinRadius The radius of the innermost ring. Must be greater than or equal to 0.
	 * @param MaxRadius The radius of the outermost ring. Must be greater than MinRadius.
	 * @param RingDeltaAngle The rotational offset applied to each ring in degrees.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateConcentricRingsTransforms(const FTransform& Origin,
	                                               const int32 RingsAmount,
	                                               const int32 PointsPerRing,
	                                               const float MinRadius,
	                                               const float MaxRadius,
	                                               const float RingDeltaAngle,
	                                               const EPointDirection Direction,
	                                               TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms for points arranged in multiple concentric rings around a given origin.
	 * Each new ring will have more points than a previous one clamped between MinPoints and MaxPoints.
	 *
	 * @param Origin Centre of the pattern.
	 * @param RingsAmount The total number of concentric rings to generate. Must be greater than 0.
	 * @param MinPoints The minimum number of points per ring. Must be greater than 0.
	 * @param MaxPoints The maximum number of points per ring. Must be greater than MinPoints.
	 * @param MinRadius The radius of the innermost ring. Must be greater than or equal to 0.
	 * @param MaxRadius The radius of the outermost ring. Must be greater than MinRadius.
	 * @param RingDeltaAngle The rotational offset applied to each ring in degrees.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateDynamicConcentricRingsTransforms(const FTransform& Origin,
	                                                      const int32 RingsAmount,
	                                                      const int32 MinPoints,
	                                                      const int32 MaxPoints,
	                                                      const float MinRadius,
	                                                      const float MaxRadius,
	                                                      const float RingDeltaAngle,
	                                                      const EPointDirection Direction,
	                                                      TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms arranged in a cylinder pattern around a given origin.
	 *
	 * @param Origin Centre of the pattern.
	 * @param RingsAmount The number of rings in the cylinder. Must be greater than 0.
	 * @param PointsPerRing The number of points in each ring. Must be greater than 0.
	 * @param Radius The radius of the cylinder. Must be greater than 0.
	 * @param Height The height of the cylinder, determining the vertical displacement between rings. Must be greater than 0.
	 * @param RingDeltaAngle The rotational offset applied to each ring in degrees.
     * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateCylinderTransforms(const FTransform& Origin,
	                                        const int32 RingsAmount,
	                                        const int32 PointsPerRing,
	                                        const float Radius,
	                                        const float Height,
	                                        const float RingDeltaAngle,
	                                        const EPointDirection Direction,
	                                        TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms arranged in a arc pattern around a given origin.
	 * 
	 * @param Origin Centre of the pattern.
	 * @param PointsAmount The number of points to distribute along the arc. Must be greater than 0.
	 * @param Radius The radius of the cylinder. Must be greater than 0.
	 * @param AngleDeg The total angle of the arc in degrees. Must be 0 or greater.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateArcTransforms(const FTransform& Origin,
	                                   const int32 PointsAmount,
	                                   const float Radius,
	                                   const float AngleDeg,
	                                   const EPointDirection Direction,
	                                   TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms for points arranged in multiple concentric arcs around a given origin.
	 *
	 * @param Origin Centre of the pattern.
	 * @param ArcsAmount The number of concentric arcs to generate. Must be greater than 0.
	 * @param PointsPerArc The number of points in each arc. Must be greater than 0.
	 * @param MinRadius The radius of the innermost arc. Must be greater than or equal to 0.
	 * @param MaxRadius The radius of the outermost arc. Must be greater than MinRadius.
	 * @param AngleDeg The total angle of the arc in degrees. Must be 0 or greater.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateConcentricArcsTransforms(const FTransform& Origin,
	                                              const int32 ArcsAmount,
	                                              const int32 PointsPerArc,
	                                              const float MinRadius,
	                                              const float MaxRadius,
	                                              const float AngleDeg,
	                                              const EPointDirection Direction,
	                                              TArray<FTransform>& OutTransforms);


	/**
	 * Calculates transforms for points arranged in multiple concentric arcs around a given origin.
	 * Each new arc will have more points than a previous one clamped between MinPoints and MaxPoints.
	 *
	 * @param Origin Centre of the pattern.
	 * @param ArcsAmount The number of concentric arcs to generate. Must be greater than 0.
	 * @param MinPoints The minimum number of points per ring. Must be greater than 0.
	 * @param MaxPoints The maximum number of points per ring. Must be greater than MinPoints.
	 * @param MinRadius The radius of the innermost arc. Must be greater than or equal to 0.
	 * @param MaxRadius The radius of the outermost arc. Must be greater than MinRadius.
	 * @param AngleDeg The total angle of the arc in degrees. Must be 0 or greater.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateDynamicConcentricArcsTransforms(const FTransform& Origin,
	                                                     const int32 ArcsAmount,
	                                                     const int32 MinPoints,
	                                                     const int32 MaxPoints,
	                                                     const float MinRadius,
	                                                     const float MaxRadius,
	                                                     const float AngleDeg,
	                                                     const EPointDirection Direction,
	                                                     TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms arranged in a cylinder pattern using arcs around a given origin.
	 *
	 * @param Origin Centre of the pattern.
	 * @param ArcsAmount The number of concentric arcs to generate. Must be greater than 0.
	 * @param PointsPerArc The number of points in each arc. Must be greater than 0.
	 * @param Radius The radius of the cylinder. Must be greater than 0.
	 * @param Height The height of the cylinder, determining the vertical displacement between rings. Must be greater than 0.
	 * @param AngleDeg The total angle of the arc in degrees. Must be 0 or greater.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateArcCylinderTransforms(const FTransform& Origin,
	                                           const int32 ArcsAmount,
	                                           const int32 PointsPerArc,
	                                           const float Radius,
	                                           const float Height,
	                                           const float AngleDeg,
	                                           const EPointDirection Direction,
	                                           TArray<FTransform>& OutTransforms);

	/**
	 * Calculates transforms uniformly distributed on the surface of a sphere around a given origin.
	 *
	 *
	 * @param Origin Centre of the sphere.
	 * @param Radius The radius of the sphere. Must be a non-negative value.
	 * @param PointsAmount The number of points. Must be greater than 0.
	 * @param MinLatitude The minimum latitude, normalized between -1 and 1, defining the vertical range of points.
	 * @param MaxLatitude The maximum latitude, normalized between -1 and 1, defining the vertical range of points.
	 * @param MinLongitude The minimum longitude, normalized between 0 and 1, defining the horizontal range of points.
	 * @param MaxLongitude The maximum longitude, normalized between 0 and 1, defining the horizontal range of points.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
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

	/**
	 * Calculates transforms arranged in a sunflower pattern using arcs around a given origin.
	 *
	 * @param Origin Centre of the pattern.
	 * @param Radius Radius of the pattern. Must be greater than 0.
	 * @param PointsAmount The number of points in the pattern. Must be greater than zero.
	 * @param Direction Specifies the orientation of each point relative to the origin.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void CalculateSunFlowerTransforms(const FTransform& Origin,
	                                         const float Radius,
	                                         const int32 PointsAmount,
	                                         const EPointDirection Direction,
	                                         TArray<FTransform>& OutTransforms);

	/**
	 * Retrieves transforms evenly distributed along a given spline based on the number of specified points.
	 *
	 * @param SplineComponent Pointer to a USplineComponent. Must be a valid instance.
	 * @param PointsAmount The number of points to place along the spline. Must be greater than 0.
	 * @param bUseLocalSpace If true, local space will be used to get transforms, otherwise world space.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void GetTransformsAlongSplineByPoints(USplineComponent* SplineComponent,
	                                             const int32 PointsAmount,
	                                             const bool bUseLocalSpace,
	                                             TArray<FTransform>& OutTransforms);

	/**
	 * Retrieves locations evenly distributed along a given spline based on the number of specified points.
	 *
	 * @param SplineComponent Pointer to a USplineComponent. Must be a valid instance.
	 * @param PointsAmount The number of points to place along the spline. Must be greater than 0.
	 * @param bUseLocalSpace If true, local space will be used to get transforms, otherwise world space.
	 * @param OutLocations An array that will store the resulting locations.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void GetLocationsAlongSplineByPoints(USplineComponent* SplineComponent,
	                                            const int32 PointsAmount,
	                                            const bool bUseLocalSpace,
	                                            TArray<FVector>& OutLocations);

	/**
	 * Retrieves transforms spaced along a spline at regular intervals determined by the specified distance.
	 *
	 * @param SplineComponent Pointer to a USplineComponent. Must be a valid instance.
	 * @param Distance The distance interval between points along the spline. Must be greater than 0.
	 * @param bUseLocalSpace If true, local space will be used to get transforms, otherwise world space.
	 * @param OutTransforms An array that will store the resulting transforms.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void GetTransformsAlongSplineByDistance(USplineComponent* SplineComponent,
	                                               const float Distance,
	                                               const bool bUseLocalSpace,
	                                               TArray<FTransform>& OutTransforms);

	/**
	 * Retrieves locations spaced along a spline at regular intervals determined by the specified distance.
	 *
	 * @param SplineComponent Pointer to a USplineComponent. Must be a valid instance.
	 * @param Distance The distance interval between points along the spline. Must be greater than 0.
	 * @param bUseLocalSpace If true, local space will be used to get transforms, otherwise world space.
	 * @param OutLocations An array that will store the resulting locations.
	 */
	UFUNCTION(BlueprintCallable, Category="TrickyUtilities")
	static void GetLocationsAlongSplineByDistance(USplineComponent* SplineComponent,
	                                              const float Distance,
	                                              const bool bUseLocalSpace,
	                                              TArray<FVector>& OutLocations);

private:
	static void CalculatePointRotation(const FTransform& Origin,
	                                   const FTransform& Point,
	                                   const EPointDirection Direction,
	                                   FRotator& OutRotation);
};
