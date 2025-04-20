// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "TrickyAnnotationActor.generated.h"

enum EHorizTextAligment : int;
enum EVerticalTextAligment : int;
class UTextRenderComponent;

/**
 * A customizable annotation actor used for displaying a custom text.
 * Useful to create annotations in levels.
 */
UCLASS(HideCategories=(Actor, Collision, Input, Replication, HLOD, WorldPartition, DataLayers, Physics,
	Networking, LevelInstance))
class TRICKYUTILITIES_API ATrickyAnnotationActor : public AActor
{
	GENERATED_BODY()

public:
	ATrickyAnnotationActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	UFUNCTION(BlueprintGetter, Category = "TrickyAnnotation")
	FText GetAnnotationText() const { return AnnotationText; };

	UFUNCTION(BlueprintSetter, Category = "TrickyAnnotation")
	void SetAnnotationText(const FText& NewText);

	UFUNCTION(BlueprintGetter, Category = "TrickyAnnotation")
	FColor GetTextColor() const { return TextColor; };

	UFUNCTION(BlueprintSetter, Category = "TrickyAnnotation")
	void SetTextColor(const FColor& NewColor);

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> FrontMeshComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BackMeshComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UTextRenderComponent> FrontTextComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UTextRenderComponent> BackTextComponent = nullptr;

	/**
	 * Text content displayed by the actor.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetAnnotationText,
		BlueprintSetter=SetAnnotationText,
		Category = "TrickyAnnotation",
		meta=(MultiLine))
	FText AnnotationText = FText::FromString("Annotation Text");

	/**
	 * Determines size of the annotation text.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category = "TrickyAnnotation",
		meta=(ClampMin=1, UIMin=1, ClampMax=128, UIMax=128))
	int32 TextSize = 32;

	/**
	 * Determines annotation text color.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetTextColor,
		BlueprintSetter=SetTextColor,
		Category = "TrickyAnnotation",
		meta=(HideAlphaChannel))
	FColor TextColor = FColor::White;

	/**
	 * Text vertical alignment.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrickyAnnotation")
	TEnumAsByte<EVerticalTextAligment> VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;

	/**
	 * Text horizontal alignment.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrickyAnnotation")
	TEnumAsByte<EHorizTextAligment> HorizontalAlignment = EHorizTextAligment::EHTA_Center;

	/**
	 * Determines text material.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrickyAnnotation", AdvancedDisplay)
	UMaterialInterface* TextMaterial = nullptr;

private:
	const float DisplacementFactor = 0.5f;

	const float TextOffset = 0.25f;
	
	UFUNCTION()
	void UpdateTextProperties(UTextRenderComponent* Component,
	                          const FText& Text,
	                          const int32 Size,
	                          const FColor& Color,
	                          const EVerticalTextAligment& VertTextAlignment,
	                          const EHorizTextAligment& HorizTextAlignment,
	                          UMaterialInterface* Material);

	UFUNCTION()
	void UpdateBackgroundSize(UStaticMeshComponent* Component) const;

	UFUNCTION()
	void UpdateTextHorizontalPosition(UTextRenderComponent* Component, const bool bNegate) const;

	UFUNCTION()
	void UpdateTextVerticalPosition(UTextRenderComponent* Component) const;

	UFUNCTION()
	void SetupComponent(UPrimitiveComponent* Component) const;
};
