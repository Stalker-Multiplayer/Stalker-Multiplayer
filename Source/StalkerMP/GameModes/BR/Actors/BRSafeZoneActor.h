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

	UPROPERTY()
		FRichCurve ChangingSizeRichCurve;

	UPROPERTY(ReplicatedUsing = OnRep_PlaybackPosition)
		float PlaybackPosition = -1;

	UPROPERTY()
		bool PlaybackPositionReplicated = false;

	UPROPERTY(ReplicatedUsing = OnRep_Params)
		FVector4 Params;

	UPROPERTY()
		bool ParamsReplicated = false;


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
		void OnRep_PlaybackPosition();

	UFUNCTION()
		void OnRep_Params();

	UFUNCTION()
		void OnEverythingReplicated();

	UFUNCTION()
		void UpdateSize(float Size);


public:

	UFUNCTION()
		void SetParams(float FullSize, float FinalSize, int ExpandDuration, int ShrinkDuration, bool SpawnArtefact);


};
