// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapPOIActor.generated.h"

class UMapPOIWidget;
class UTooltipWidget;

class UBoxComponent;

UCLASS()
class STALKERMP_API AMapPOIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapPOIActor();




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* POIBox;


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UMapPOIWidget> POIWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UTooltipWidget> TooltipWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool MatchBoxSize = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool RotateAccordingToBox = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText TooltipTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = "true"))
		FText TooltipText;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		FVector GetScaledBoxSize();


};
