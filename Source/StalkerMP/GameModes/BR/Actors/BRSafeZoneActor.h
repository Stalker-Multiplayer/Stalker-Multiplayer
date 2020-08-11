// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "BRSafeZoneActor.generated.h"

UCLASS()
class STALKERMP_API ABRSafeZoneActor : public AActor
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	ABRSafeZoneActor();

	virtual void BeginPlay() override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UTimelineComponent* ChangingSizeTimeline;

	UPROPERTY()
		UCurveFloat* ChangingSizeCurve;

	UPROPERTY(ReplicatedUsing = OnRep_ChangingSizeRichCurve)
		FRichCurve ChangingSizeRichCurve;

	UPROPERTY(ReplicatedUsing = OnRep_TimelineLength)
		int TimelineLength = -1;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMeshOutside;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMeshInside;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ABaseItem> ArtefactItemClass;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnRep_ChangingSizeRichCurve();

	UFUNCTION()
		void OnRep_TimelineLength();

	UFUNCTION()
		void UpdateSize(float Size);


public:

	UFUNCTION()
		void SetParams(float FullSize, float FinalSize, int ExpandDuration, int ShrinkDuration, bool SpawnArtefact);


};
