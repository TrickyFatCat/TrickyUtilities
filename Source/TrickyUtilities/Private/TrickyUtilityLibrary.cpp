// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyUtilityLibrary.h"

#include "Components/SplineComponent.h"
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
                                              const EStringTimeFormat TimeFormat,
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
	case EStringTimeFormat::MM_SS_MsMs:
		OutResult = FString::Printf(TEXT("%02d:%02d.%02d"),
		                            TotalMinutes,
		                            Seconds,
		                            ConvertMilliseconds(0.1f));
		break;

	case EStringTimeFormat::MM_SS_Ms:
		OutResult = FString::Printf(TEXT("%02d:%02d.%d"),
		                            TotalMinutes,
		                            Seconds,
		                            ConvertMilliseconds(0.01f));
		break;

	case EStringTimeFormat::MM_SS:
		OutResult = FString::Printf(TEXT("%02d:%02d"), TotalMinutes, Seconds);
		break;

	case EStringTimeFormat::SS_MsMs:
		OutResult = FString::Printf(TEXT("%02d.%02d"), TotalSeconds, ConvertMilliseconds(0.1f));
		break;

	case EStringTimeFormat::SS_Ms:
		OutResult = FString::Printf(TEXT("%02d.%d"), TotalSeconds, ConvertMilliseconds(0.01f));
		break;

	case EStringTimeFormat::SS:
		OutResult = FString::Printf(TEXT("%02d"), TotalSeconds);
		break;
	}
}

void UTrickyUtilityLibrary::CalculateGridTransforms(const FTransform& Origin,
                                                    const int32 SizeX,
                                                    const int32 SizeY,
                                                    const FVector2D& CellSize,
                                                    TArray<FTransform>& OutTransforms)
{
	if (SizeX <= 0 || SizeY <= 0 || CellSize.X <= 0.f || CellSize.Y <= 0.f)
	{
		return;
	}
	
	const FVector OriginFwdVec = Origin.GetRotation().GetForwardVector();
	const FVector OriginRightVec = Origin.GetRotation().GetRightVector();

	FTransform NewTransform = FTransform::Identity;

	for (int32 x = 0; x < SizeX; ++x)
	{
		for (int32 y = 0; y < SizeY; ++y)
		{
			FVector NewLocation = FVector::ZeroVector;
			NewLocation += x * CellSize.X * OriginFwdVec;
			NewLocation += y * CellSize.Y * OriginRightVec;
			NewTransform.SetLocation(NewLocation);
			OutTransforms.Emplace(Origin * NewTransform);
		}
	}
}

void UTrickyUtilityLibrary::CalculateCubeTransforms(const FTransform& Origin,
                                                    const int32 SizeX,
                                                    const int32 SizeY,
                                                    const int32 SizeZ,
                                                    const FVector& CellSize,
                                                    TArray<FTransform>& OutTransforms)
{
	if (SizeX <= 0 || SizeY <= 0 || SizeZ <= 0 || CellSize.X <= 0.f || CellSize.Y <= 0.f || CellSize.Z <= 0.f)
	{
		return;
	}
	
	const FVector OriginFwdVec = Origin.GetRotation().GetForwardVector();
	const FVector OriginRightVec = Origin.GetRotation().GetRightVector();
	const FVector OriginUpVec = Origin.GetRotation().GetUpVector();

	FTransform NewTransform = FTransform::Identity;

	for (int32 x = 0; x < SizeX; ++x)
	{
		for (int32 y = 0; y < SizeY; ++y)
		{
			for (int32 z = 0; z < SizeZ; ++z)
			{
				FVector NewLocation = FVector::ZeroVector;
				NewLocation += x * CellSize.X * OriginFwdVec;
				NewLocation += y * CellSize.Y * OriginRightVec;
				NewLocation += z * CellSize.Z * OriginUpVec;
				NewTransform.SetLocation(NewLocation);
				OutTransforms.Emplace(Origin * NewTransform);
			}
		}
	}
}

void UTrickyUtilityLibrary::CalculateRingTransform(const FTransform& Origin,
                                                   const int32 PointsAmount,
                                                   const float Radius,
                                                   const EPointDirection Direction,
                                                   TArray<FTransform>& OutTransforms)
{
	if (PointsAmount <= 0 || Radius < 0.f)
	{
		return;
	}

	const float Theta = (2.f * PI) / PointsAmount;
	FTransform NewTransform = FTransform::Identity;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float Angle = Theta * i;
		FVector NewLocation = FVector::ZeroVector;
		NewLocation.X = Radius * FMath::Cos(Angle);
		NewLocation.Y = Radius * FMath::Sin(Angle);
		NewTransform.SetLocation(NewLocation);
		NewTransform *= Origin;

		FRotator PointRotation = FRotator::ZeroRotator;
		CalculatePointRotation(Origin, NewTransform, Direction, PointRotation);
		NewTransform.SetRotation(PointRotation.Quaternion());

		OutTransforms.Emplace(NewTransform);
	}
}

void UTrickyUtilityLibrary::CalculateConcentricRingsTransforms(const FTransform& Origin,
                                                               const int32 RingsAmount,
                                                               const int32 PointsPerRing,
                                                               const float MinRadius,
                                                               const float MaxRadius,
                                                               const float RingDeltaAngle,
                                                               const EPointDirection Direction,
                                                               TArray<FTransform>& OutTransforms)
{
	if (RingsAmount <= 0 || PointsPerRing <= 0 || MinRadius < 0.f || MaxRadius <= MinRadius)
	{
		return;
	}

	const float RadiusStep = (MaxRadius - MinRadius) / (RingsAmount - 1);
	FTransform RingOrigin = Origin;
	const FVector OriginFwdVector = RingOrigin.GetRotation().GetForwardVector();
	const FVector OriginUpVector = RingOrigin.GetRotation().GetUpVector();

	for (int32 i = 0; i < RingsAmount; ++i)
	{
		const FVector XAxis = OriginFwdVector.RotateAngleAxis(RingDeltaAngle * i, OriginUpVector);
		const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(XAxis, OriginUpVector);
		const FRotator PointRotation = RotationMatrix.Rotator();
		RingOrigin.SetRotation(PointRotation.Quaternion());

		const float RingRadius = MinRadius + RadiusStep * i;
		
		TArray<FTransform> RingTransforms;
		CalculateRingTransform(RingOrigin, PointsPerRing, RingRadius, Direction, RingTransforms);
		OutTransforms.Append(RingTransforms);
	}
}

void UTrickyUtilityLibrary::CalculateDynamicConcentricRingsTransforms(const FTransform& Origin,
                                                                      const int32 RingsAmount,
                                                                      const int32 MinPoints,
                                                                      const int32 MaxPoints,
                                                                      const float MinRadius,
                                                                      const float MaxRadius,
                                                                      const float RingDeltaAngle,
                                                                      const EPointDirection Direction,
                                                                      TArray<FTransform>& OutTransforms)
{
	if (RingsAmount <= 0 || MinPoints <= 0 || MaxPoints <= MinPoints || MinRadius < 0.f || MaxRadius <= MinRadius)
	{
		return;
	}

	const float RadiusStep = (MaxRadius - MinRadius) / (RingsAmount - 1);
	FTransform RingOrigin = Origin;
	const FVector OriginFwdVector = RingOrigin.GetRotation().GetForwardVector();
	const FVector OriginUpVector = RingOrigin.GetRotation().GetUpVector();

	for (int32 i = 0; i < RingsAmount; ++i)
	{
		const FVector XAxis = OriginFwdVector.RotateAngleAxis(RingDeltaAngle * i, OriginUpVector);
		const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(XAxis, OriginUpVector);
		const FRotator PointRotation = RotationMatrix.Rotator();
		RingOrigin.SetRotation(PointRotation.Quaternion());

		TArray<FTransform> RingTransforms;
		const float RingRadius = MinRadius + RadiusStep * i;

		int32 PointsAmount = MinPoints;
		if (i > 0 && MinRadius > 0.f)
		{
			const float CircumferenceRatio = RingRadius / MinRadius;
			PointsAmount = FMath::Clamp(FMath::RoundToInt(MinPoints * CircumferenceRatio), MinPoints, MaxPoints);
		}

		CalculateRingTransform(RingOrigin, PointsAmount, RingRadius, Direction, RingTransforms);
		OutTransforms.Append(RingTransforms);
	}
}

void UTrickyUtilityLibrary::CalculateCylinderTransforms(const FTransform& Origin,
                                                        const int32 RingsAmount,
                                                        const int32 PointsPerRing,
                                                        const float Radius,
                                                        const float Height,
                                                        const float RingDeltaAngle,
                                                        const EPointDirection Direction,
                                                        TArray<FTransform>& OutTransforms)
{
	if (RingsAmount <= 0 || PointsPerRing <= 0 || Radius < 0.f || Height < 0.f)
	{
		return;
	}

	FTransform RingOrigin = Origin;
	const FVector OriginLocation = RingOrigin.GetLocation();
	const FVector OriginFwdVector = RingOrigin.GetRotation().GetForwardVector();
	const FVector OriginUpVector = RingOrigin.GetRotation().GetUpVector();
	const float RingDisplacement = Height / (RingsAmount - 1);

	for (int32 i = 0; i < RingsAmount; ++i)
	{
		RingOrigin.SetLocation(OriginLocation + OriginUpVector * RingDisplacement * i);
		const FVector XAxis = OriginFwdVector.RotateAngleAxis(RingDeltaAngle * i, OriginUpVector);
		const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(XAxis, OriginUpVector);
		const FRotator PointRotation = RotationMatrix.Rotator();
		RingOrigin.SetRotation(PointRotation.Quaternion());
		
		TArray<FTransform> RingTransforms;
		CalculateRingTransform(RingOrigin, PointsPerRing, Radius, Direction, RingTransforms);
		OutTransforms.Append(RingTransforms);
	}
}

void UTrickyUtilityLibrary::CalculateArcTransforms(const FTransform& Origin,
                                                   const int32 PointsAmount,
                                                   const float Radius,
                                                   const float AngleDeg,
                                                   const EPointDirection Direction,
                                                   TArray<FTransform>& OutTransforms)
{
	if (PointsAmount <= 0 || Radius < 0.f || AngleDeg < 0.f)
	{
		return;
	}

	const float AngleRad = UKismetMathLibrary::DegreesToRadians(AngleDeg);
	const float Theta = AngleRad / (PointsAmount - 1);

	FTransform ArcOrigin = Origin;
	const FVector OriginFwdVector = ArcOrigin.GetRotation().GetForwardVector();
	const FVector OriginUpVector = Origin.GetRotation().GetUpVector();

	const FVector XAxis = OriginFwdVector.RotateAngleAxis(-AngleDeg * 0.5f, OriginUpVector);
	const FMatrix ArcRotationMatrix = FRotationMatrix::MakeFromXZ(XAxis, OriginUpVector);
	const FRotator ArcRotation = ArcRotationMatrix.Rotator();
	ArcOrigin.SetRotation(ArcRotation.Quaternion());

	FTransform NewTransform = FTransform::Identity;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float Angle = Theta * i;
		FVector NewLocation = FVector::ZeroVector;
		NewLocation.X = Radius * FMath::Cos(Angle);
		NewLocation.Y = Radius * FMath::Sin(Angle);
		NewTransform.SetLocation(NewLocation);
		NewTransform *= ArcOrigin;

		FRotator PointRotation = FRotator::ZeroRotator;
		CalculatePointRotation(ArcOrigin, NewTransform, Direction, PointRotation);
		NewTransform.SetRotation(PointRotation.Quaternion());

		OutTransforms.Emplace(NewTransform);
	}
}

void UTrickyUtilityLibrary::CalculateConcentricArcsTransforms(const FTransform& Origin,
                                                              const int32 ArcsAmount,
                                                              const int32 PointsPerArc,
                                                              const float MinRadius,
                                                              const float MaxRadius,
                                                              const float AngleDeg,
                                                              const EPointDirection Direction,
                                                              TArray<FTransform>& OutTransforms)
{
	if (ArcsAmount <= 0 || PointsPerArc <= 0 || MinRadius < 0.f || MaxRadius <= MinRadius)
	{
		return;
	}

	const float RadiusStep = (MaxRadius - MinRadius) / (ArcsAmount - 1);
	for (int32 i = 0; i < ArcsAmount; ++i)
	{
		const float ArcRadius = MinRadius + RadiusStep * i;
		
		TArray<FTransform> RingTransforms;
		CalculateArcTransforms(Origin, PointsPerArc, ArcRadius, AngleDeg, Direction, RingTransforms);
		OutTransforms.Append(RingTransforms);
	}
}

void UTrickyUtilityLibrary::CalculateDynamicConcentricArcsTransforms(const FTransform& Origin,
                                                                     const int32 ArcsAmount,
                                                                     const int32 MinPoints,
                                                                     const int32 MaxPoints,
                                                                     const float MinRadius,
                                                                     const float MaxRadius,
                                                                     const float AngleDeg,
                                                                     const EPointDirection Direction,
                                                                     TArray<FTransform>& OutTransforms)
{
	if (ArcsAmount <= 0 || MinPoints <= 0 || MaxPoints <= MinPoints || MinRadius < 0.f || MaxRadius <= MinRadius)
	{
		return;
	}

	const float RadiusStep = (MaxRadius - MinRadius) / (ArcsAmount - 1);
	
	for (int32 i = 0; i < ArcsAmount; ++i)
	{
		const float ArcRadius = MinRadius + RadiusStep * i;

		int32 PointsAmount = MinPoints;

		if (i > 0 && MinRadius > 0.f)
		{
			const float CircumferenceRatio = ArcRadius / MinRadius;
			PointsAmount = FMath::Clamp(FMath::RoundToInt(MinPoints * CircumferenceRatio), MinPoints, MaxPoints);
		}

		TArray<FTransform> RingTransforms;
		CalculateArcTransforms(Origin, PointsAmount, ArcRadius, AngleDeg, Direction, RingTransforms);
		OutTransforms.Append(RingTransforms);
	}
}

void UTrickyUtilityLibrary::CalculateArcCylinderTransforms(const FTransform& Origin,
                                                           const int32 ArcsAmount,
                                                           const int32 PointsPerArc,
                                                           const float Radius,
                                                           const float Height,
                                                           const float AngleDeg,
                                                           const EPointDirection Direction,
                                                           TArray<FTransform>& OutTransforms)
{
	if (ArcsAmount <= 0 || PointsPerArc <= 0 || Radius < 0.f || Height < 0.f)
	{
		return;
	}

	FTransform ArcOrigin = Origin;
	const FVector OriginLocation = ArcOrigin.GetLocation();
	const FVector OriginUpVector = ArcOrigin.GetRotation().GetUpVector();
	const float ArcDisplacement = Height / (ArcsAmount - 1);

	for (int32 i = 0; i < ArcsAmount; ++i)
	{
		ArcOrigin.SetLocation(OriginLocation + OriginUpVector * ArcDisplacement * i);
		
		TArray<FTransform> ArcsTransforms;
		CalculateArcTransforms(ArcOrigin, PointsPerArc, Radius, AngleDeg, Direction, ArcsTransforms);
		OutTransforms.Append(ArcsTransforms);
	}
}

void UTrickyUtilityLibrary::CalculateSphereTransforms(const FTransform& Origin,
                                                      const float Radius,
                                                      const int32 PointsAmount,
                                                      const float MinLatitude,
                                                      const float MaxLatitude,
                                                      const float MinLongitude,
                                                      const float MaxLongitude,
                                                      const EPointDirection Direction,
                                                      TArray<FTransform>& OutTransforms)
{
	if (PointsAmount <= 0 || Radius < 0.f)
	{
		return;
	}

	const float Phi = PI * (3.f - FMath::Sqrt(5.f));
	constexpr float Tau = PI * 2;

	const float MinLongitudeAngle = MinLongitude * 360.f;
	const float MaxLongitudeAngle = MaxLongitude * 360.f;
	const float MinLongitudeRad = FMath::DegreesToRadians(MinLongitudeAngle);
	const float MaxLongitudeRad = FMath::DegreesToRadians(MaxLongitudeAngle);

	FTransform NewTransform = FTransform::Identity;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float Z = ((i / (static_cast<float>(PointsAmount) - 1.f)) * (MaxLatitude - MinLatitude) + MinLatitude) *
			2.f - 1.f;
		const float RadiusZ = FMath::Sqrt(1.f - Z * Z);
		float Theta = Phi * static_cast<float>(i);

		if (MinLongitudeAngle != 0.f || MaxLongitudeAngle != 360.f)
		{
			Theta = FMath::Fmod(Theta, Tau);
			Theta = Theta < 0 ? Theta + Tau : Theta;
			Theta = Theta * MaxLongitudeRad / Tau + MinLongitudeRad;
		}

		const float X = FMath::Sin(Theta) * RadiusZ;
		const float Y = FMath::Cos(Theta) * RadiusZ;

		FVector Location{X, Y, Z};
		Location *= Radius;
		Location = Origin.TransformPosition(Location);
		NewTransform.SetLocation(Location);

		FRotator PointRotation = FRotator::ZeroRotator;
		CalculatePointRotation(Origin, NewTransform, Direction, PointRotation);
		NewTransform.SetRotation(PointRotation.Quaternion());

		OutTransforms.Emplace(NewTransform);
	}
}

void UTrickyUtilityLibrary::CalculateSunFlowerTransforms(const FTransform& Origin,
                                                         const float Radius,
                                                         const int32 PointsAmount,
                                                         const EPointDirection Direction,
                                                         TArray<FTransform>& OutTransforms)
{
	if (PointsAmount <= 0 || Radius < 0.f)
	{
		return;
	}

	const float GoldenAngle = PI * (3.0f - FMath::Sqrt(5.0f));
	FTransform NewTransform = FTransform::Identity;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float PointRadius = Radius * FMath::Sqrt(static_cast<float>(i) / static_cast<float>(PointsAmount));
		const float Angle = i * GoldenAngle;

		FVector NewLocation = FVector::ZeroVector;
		NewLocation.X = PointRadius * FMath::Cos(Angle);
		NewLocation.Y = PointRadius * FMath::Sin(Angle);
		NewTransform.SetLocation(NewLocation);
		NewTransform *= Origin;

		FRotator PointRotation = FRotator::ZeroRotator;
		CalculatePointRotation(Origin, NewTransform, Direction, PointRotation);
		NewTransform.SetRotation(PointRotation.Quaternion());

		OutTransforms.Emplace(NewTransform);
	}
}

void UTrickyUtilityLibrary::GetTransformsAlongSplineByPoints(USplineComponent* SplineComponent,
                                                             const int32 PointsAmount,
                                                             const bool bUseLocalSpace,
                                                             TArray<FTransform>& OutTransforms)
{
	if (!IsValid(SplineComponent) || PointsAmount <= 0)
	{
		return;
	}

	const float SplineLength = SplineComponent->GetSplineLength();
	const float Displacement = SplineLength / (PointsAmount - 1);

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float Distance = Displacement * i;
		const ESplineCoordinateSpace::Type CoordinateSpace = bUseLocalSpace
			                                                     ? ESplineCoordinateSpace::Local
			                                                     : ESplineCoordinateSpace::World;
		const FTransform Transform = SplineComponent->GetTransformAtDistanceAlongSpline(Distance, CoordinateSpace);
		OutTransforms.Emplace(Transform);
	}
}

void UTrickyUtilityLibrary::GetLocationsAlongSplineByPoints(USplineComponent* SplineComponent,
                                                            const int32 PointsAmount,
                                                            const bool bUseLocalSpace,
                                                            TArray<FVector>& OutLocations)
{
	if (!IsValid(SplineComponent) || PointsAmount <= 0)
	{
		return;
	}

	const float SplineLength = SplineComponent->GetSplineLength();
	const float Displacement = SplineLength / (PointsAmount - 1);
	const ESplineCoordinateSpace::Type CoordinateSpace = bUseLocalSpace
		                                                     ? ESplineCoordinateSpace::Local
		                                                     : ESplineCoordinateSpace::World;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float Distance = Displacement * i;
		const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, CoordinateSpace);
		OutLocations.Emplace(Location);
	}
}

void UTrickyUtilityLibrary::GetTransformsAlongSplineByDistance(USplineComponent* SplineComponent,
                                                               const float Distance,
                                                               const bool bUseLocalSpace,
                                                               TArray<FTransform>& OutTransforms)
{
	if (!IsValid(SplineComponent) || Distance <= 0.f)
	{
		return;
	}

	const float SplineLength = SplineComponent->GetSplineLength();
	const float PointsAmount = SplineLength / Distance;
	const float HalfDistance = Distance * 0.5f;
	const ESplineCoordinateSpace::Type CoordinateSpace = bUseLocalSpace
		                                                     ? ESplineCoordinateSpace::Local
		                                                     : ESplineCoordinateSpace::World;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float SplineDistance = (Distance * i) + HalfDistance;
		const FTransform Transform = SplineComponent->GetTransformAtDistanceAlongSpline(SplineDistance, CoordinateSpace);
		OutTransforms.Emplace(Transform);
	}
}

void UTrickyUtilityLibrary::GetLocationsAlongSplineByDistance(USplineComponent* SplineComponent,
                                                              const float Distance,
                                                              const bool bUseLocalSpace,
                                                              TArray<FVector>& OutLocations)
{
	if (!IsValid(SplineComponent) || Distance <= 0.f)
	{
		return;
	}

	const float SplineLength = SplineComponent->GetSplineLength();
	const float PointsAmount = SplineLength / Distance;
	const float HalfDistance = Distance * 0.5f;
	const ESplineCoordinateSpace::Type CoordinateSpace = bUseLocalSpace
		                                                     ? ESplineCoordinateSpace::Local
		                                                     : ESplineCoordinateSpace::World;

	for (int32 i = 0; i < PointsAmount; ++i)
	{
		const float SplineDistance = (Distance * i) + HalfDistance;
		const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(SplineDistance, CoordinateSpace);
		OutLocations.Emplace(Location);
	}
}

void UTrickyUtilityLibrary::CalculatePointRotation(const FTransform& Origin,
                                                   const FTransform& Point,
                                                   const EPointDirection Direction,
                                                   FRotator& OutRotation)
{
	const FVector WorldLocation = Point.GetLocation();
	const FVector WorldUpVector = Origin.GetRotation().GetUpVector();
	const FVector OriginLocation = Origin.GetLocation();

	FVector DirectionToOrigin = WorldLocation - OriginLocation;
	DirectionToOrigin.Normalize();

	switch (Direction)
	{
	case EPointDirection::Origin:
		OutRotation = Origin.GetRotation().Rotator();
		break;
	case EPointDirection::Clockwise:
		{
			const FVector TangentVector = FVector::CrossProduct(WorldUpVector, DirectionToOrigin);
			const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(TangentVector, WorldUpVector);
			OutRotation = RotationMatrix.Rotator();
		}
		break;
	case EPointDirection::CounterClockwise:
		{
			const FVector TangentVector = FVector::CrossProduct(DirectionToOrigin, WorldUpVector);
			const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(TangentVector, WorldUpVector);
			OutRotation = RotationMatrix.Rotator();
		}
		break;
	case EPointDirection::Inside:
		{
			DirectionToOrigin = OriginLocation - WorldLocation;
			DirectionToOrigin.Normalize();
			const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(DirectionToOrigin, WorldUpVector);
			OutRotation = RotationMatrix.Rotator();
		}
		break;
	case EPointDirection::Outside:
		{
			const FMatrix RotationMatrix = FRotationMatrix::MakeFromXZ(DirectionToOrigin, WorldUpVector);
			OutRotation = RotationMatrix.Rotator();
		}
		break;
	}
}
