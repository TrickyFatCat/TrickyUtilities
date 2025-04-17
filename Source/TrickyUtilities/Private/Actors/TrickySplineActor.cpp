// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Actors/TrickySplineActor.h"

#include "Components/SplineComponent.h"


ATrickySplineActor::ATrickySplineActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}
