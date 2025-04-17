// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrickySplineActor.generated.h"

class USplineComponent;

UCLASS()
class TRICKYUTILITIES_API ATrickySplineActor : public AActor
{
	GENERATED_BODY()

public:
	ATrickySplineActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;
};
