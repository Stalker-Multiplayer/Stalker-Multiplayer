// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Curves/RichCurve.h"
#include "BRZoneActor.generated.h"

class APlayerCharacter;
class UCurveFloat;

UCLASS()
class STALKERMP_API ABRZoneActor : public AActor
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

public:

	static const FString ZONE_STAGE_AMOUNT_SETTING_KEY;
	static const FString ZONE_STAGE_FINAL_SIZE_SETTING_KEY;
	static const FString ZONE_STAGE_WAIT_TIME_SETTING_KEY;
	static const FString ZONE_STAGE_SHRINK_TIME_SETTING_KEY;
	static const FString ZONE_STAGE_DAMAGE_SETTING_KEY;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	// Sets default values for this actor's properties
	ABRZoneActor();

	virtual void BeginPlay() override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		FTimerHandle DamageTimerHandle;

	UPROPERTY()
		FTimerHandle ZoneWaitTimerHandle;

	UPROPERTY()
		FTimerHandle ZoneMovingTimerHandle;

	UPROPERTY()
		UTimelineComponent* ZoneTimeline;

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	UPROPERTY()
		UCurveFloat* ZoneSizeCurve;

	UPROPERTY()
		UCurveVector* ZoneLocationCurve;

	UPROPERTY(ReplicatedUsing = OnRep_ZoneTimeLineLength)
		float ZoneTimeLineLength;

	UPROPERTY(ReplicatedUsing = OnRep_ZoneSizeRichCurve)
		FRichCurve ZoneSizeRichCurve;

	UPROPERTY(ReplicatedUsing = OnRep_ZoneLocationRichCurveX)
		FRichCurve ZoneLocationRichCurveX;

	UPROPERTY(ReplicatedUsing = OnRep_ZoneLocationRichCurveY)
		FRichCurve ZoneLocationRichCurveY;

	UPROPERTY(ReplicatedUsing = OnRep_ZoneLocationRichCurveZ)
		FRichCurve ZoneLocationRichCurveZ;

	UPROPERTY()
		int CurrentZoneStage = -1;

	UPROPERTY()
		int MaxZoneStage = -1;

	UPROPERTY()
		float CurrentZoneDamage = 0;

	UPROPERTY()
		FVector InitialZoneLocation;

	UPROPERTY()
		float InitialZoneSize = 0;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector4> SIZE_WAIT_SHRINK_DAMAGE;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void StartNextZoneStage();

	UFUNCTION()
		void OnZoneShrinkStarted();

	UFUNCTION()
		void DamagePlayerOutsideZone();

	UFUNCTION()
		void OnRep_ZoneTimeLineLength();

	UFUNCTION()
		void OnRep_ZoneSizeRichCurve();

	UFUNCTION()
		void OnRep_ZoneLocationRichCurveX();

	UFUNCTION()
		void OnRep_ZoneLocationRichCurveY();

	UFUNCTION()
		void OnRep_ZoneLocationRichCurveZ();


protected:

	UFUNCTION(BlueprintImplementableEvent)
		void ZoneSizeUpdate(float val);

	UFUNCTION(BlueprintImplementableEvent)
		void ZoneLocationUpdate(FVector val);

	UFUNCTION(BlueprintImplementableEvent)
		FVector GetNextZoneStageLocation(FVector CurrentLocation, float ZoneStageSizeDiff);


public:

	UFUNCTION()
		void Reset();

	UFUNCTION()
		void Start();

	UFUNCTION()
		void Stop();

	UFUNCTION()
		int GetTimeToFullyShrink();


	
};
