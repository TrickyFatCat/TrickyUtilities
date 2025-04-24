// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Actors/TrickyAnnotationActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"


ATrickyAnnotationActor::ATrickyAnnotationActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FrontMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontMesh"));
	FrontMeshComponent->SetupAttachment(GetRootComponent());
	SetupComponent(FrontMeshComponent);

	BackMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackMesh"));
	BackMeshComponent->SetupAttachment(GetRootComponent());
	SetupComponent(BackMeshComponent);

	if (FrontMeshComponent && BackMeshComponent)
	{
		UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(this, TEXT("/Engine/BasicShapes/Plane"));
		UMaterialInterface* Material = LoadObject<UMaterialInterface>(
			this, TEXT("/Engine/EngineDebugMaterials/BlackUnlitMaterial"));

		FrontMeshComponent->SetStaticMesh(StaticMesh);
		FrontMeshComponent->SetMaterial(0, Material);
		FrontMeshComponent->SetRelativeLocation(FVector(1, 0, 0));
		FrontMeshComponent->SetRelativeRotation(FRotator(-90, 0, 0));

		BackMeshComponent->SetStaticMesh(StaticMesh);
		BackMeshComponent->SetMaterial(0, Material);
		BackMeshComponent->SetRelativeLocation(FVector(-1, 0, 0));
		BackMeshComponent->AddRelativeRotation(FRotator(90, 0, 0));
	}

	FrontTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("FrontText"));
	FrontTextComponent->SetupAttachment(GetRootComponent());
	SetupComponent(FrontTextComponent);

	BackTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BackText"));
	BackTextComponent->SetupAttachment(GetRootComponent());
	SetupComponent(BackTextComponent);

	if (FrontTextComponent && BackTextComponent)
	{
		TextMaterial = LoadObject<UMaterialInterface>(
			this, TEXT("/Engine/EditorResources/FieldNodes/_Resources/DefaultTextMaterialOpaque"));


		FrontTextComponent->SetRelativeLocation(FVector(2, 0, 0));
		UpdateTextProperties(FrontTextComponent,
		                     AnnotationText,
		                     TextSize,
		                     TextColor,
		                     VerticalAlignment,
		                     HorizontalAlignment,
		                     TextMaterial);
		UpdateTextHorizontalPosition(FrontTextComponent, false);
		UpdateTextVerticalPosition(FrontTextComponent);

		BackTextComponent->SetRelativeLocation(FVector(-2, 0, 0));
		BackTextComponent->SetRelativeRotation(FRotator(0, 180, 0));
		UpdateTextProperties(BackTextComponent,
		                     AnnotationText,
		                     TextSize,
		                     TextColor,
		                     VerticalAlignment,
		                     HorizontalAlignment,
		                     TextMaterial);
		UpdateTextHorizontalPosition(BackTextComponent, true);
		UpdateTextVerticalPosition(BackTextComponent);

		UpdateBackgroundSize(FrontMeshComponent);
		UpdateBackgroundSize(BackMeshComponent);
	}
}

void ATrickyAnnotationActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateTextProperties(FrontTextComponent,
	                     AnnotationText,
	                     TextSize,
	                     TextColor,
	                     VerticalAlignment,
	                     HorizontalAlignment,
	                     TextMaterial);
	UpdateTextHorizontalPosition(FrontTextComponent, false);
	UpdateTextVerticalPosition(FrontTextComponent);

	UpdateTextProperties(BackTextComponent,
	                     AnnotationText,
	                     TextSize,
	                     TextColor,
	                     VerticalAlignment,
	                     HorizontalAlignment,
	                     TextMaterial);
	UpdateTextHorizontalPosition(BackTextComponent, true);
	UpdateTextVerticalPosition(BackTextComponent);

	UpdateBackgroundSize(FrontMeshComponent);
	UpdateBackgroundSize(BackMeshComponent);
}

void ATrickyAnnotationActor::SetAnnotationText(const FText& NewText)
{
	AnnotationText = NewText;
	FrontTextComponent->SetText(AnnotationText);
	BackTextComponent->SetText(AnnotationText);
}

void ATrickyAnnotationActor::SetTextColor(const FColor& NewColor)
{
	TextColor = NewColor;
	FrontTextComponent->SetTextRenderColor(TextColor);
	BackTextComponent->SetTextRenderColor(TextColor);
}

void ATrickyAnnotationActor::UpdateTextProperties(UTextRenderComponent* Component,
                                                  const FText& Text,
                                                  const int32 Size,
                                                  const FColor& Color,
                                                  const EVerticalTextAligment& VertTextAlignment,
                                                  const EHorizTextAligment& HorizTextAlignment,
                                                  UMaterialInterface* Material)
{
	if (!IsValid(Component))
	{
		return;
	}

	Component->SetText(AnnotationText);
	Component->SetTextMaterial(TextMaterial);
	Component->SetWorldSize(TextSize);
	Component->SetTextRenderColor(TextColor);
	Component->SetVerticalAlignment(VerticalAlignment);
	Component->SetHorizontalAlignment(HorizontalAlignment);

	if (Material != nullptr)
	{
		Component->SetTextMaterial(Material);
	}
}

void ATrickyAnnotationActor::UpdateBackgroundSize(UStaticMeshComponent* Component) const
{
	if (!IsValid(Component))
	{
		return;
	}

	const FVector TextLocalSize = FrontTextComponent->GetTextLocalSize();
	FVector NewScale = FVector::OneVector;
	NewScale.X = (TextLocalSize.Z / 100.f) + TextOffset;
	NewScale.Y = (TextLocalSize.Y / 100.f) + TextOffset;
	Component->SetRelativeScale3D(NewScale);
}

void ATrickyAnnotationActor::UpdateTextHorizontalPosition(UTextRenderComponent* Component, const bool bNegate) const
{
	if (!IsValid(Component))
	{
		return;
	}

	float CurrentDisplacementFactor = 0.f;

	switch (HorizontalAlignment)
	{
	case EHTA_Left:
		CurrentDisplacementFactor = DisplacementFactor;
		break;

	case EHTA_Right:
		CurrentDisplacementFactor = -DisplacementFactor;
		break;

	default:
		CurrentDisplacementFactor = 0.f;
		break;
	}

	FVector NewLocation = Component->GetRelativeLocation();
	NewLocation.Y = CurrentDisplacementFactor * Component->GetTextLocalSize().Y;

	if (bNegate)
	{
		NewLocation.Y *= -1;
	}

	Component->SetRelativeLocation(NewLocation);
}

void ATrickyAnnotationActor::UpdateTextVerticalPosition(UTextRenderComponent* Component) const
{
	if (!IsValid(Component))
	{
		return;
	}

	float CurrentDisplacementFactor = 0.f;

	switch (VerticalAlignment)
	{
	case EVRTA_TextTop:
		CurrentDisplacementFactor = DisplacementFactor;
		break;

	case EVRTA_TextBottom:
		CurrentDisplacementFactor = -DisplacementFactor;
		break;

	default:
		CurrentDisplacementFactor = 0.f;
		break;
	}

	FVector NewLocation = Component->GetRelativeLocation();
	NewLocation.Z = CurrentDisplacementFactor * Component->GetTextLocalSize().Z;
	Component->SetRelativeLocation(NewLocation);
}

void ATrickyAnnotationActor::SetupComponent(UPrimitiveComponent* Component) const
{
	if (!IsValid(Component))
	{
		return;
	}
	
	Component->SetupAttachment(GetRootComponent());
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Component->SetCollisionResponseToAllChannels(ECR_Ignore);
	Component->SetCastShadow(false);
	Component->SetReceivesDecals(false);
}
