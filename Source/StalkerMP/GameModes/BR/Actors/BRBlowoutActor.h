// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "BRBlowoutActor.generated.h"

class AWeatherActor;

class UBoxComponent;

UCLASS()
class STALKERMP_API ABRBlowoutActor : public AActor
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const FString BLOWOUT_STAGE_AMOUNT_SETTING_KEY;
	static const FString BLOWOUT_STAGE_WAIT_TIME_SETTING_KEY;
	static const FString BLOWOUT_STAGE_WARNING_TIME_SETTING_KEY;
	static const FString BLOWOUT_STAGE_DURATION_SETTING_KEY;
	static const FString BLOWOUT_STAGE_DAMAGE_SETTING_KEY;

	static const FString BLOWOUT_STAGE_SAFECOUNT_SETTING_KEY;
	static const FString BLOWOUT_STAGE_SAFESIZESTART_SETTING_KEY;
	static const FString BLOWOUT_STAGE_SAFESIZEEND_SETTING_KEY;


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	ABRBlowoutActor();

	virtual void BeginPlay() override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		AWeatherActor* WeatherActor;

	UPROPERTY()
		FTimerHandle BlowoutTimerHandle;

	UPROPERTY()
		FTimerHandle DamageTimerHandle;

	UPROPERTY()
		int CurrentBlowoutStage = -1;

	UPROPERTY()
		int MaxBlowoutStage = -1;

	UPROPERTY()
		int SecondsTillTheEnd = 0;

	UPROPERTY()
		float CurrentBlowoutDamage = 0;

	UPROPERTY()
		float CmPerPixel = 100;

	UPROPERTY()
		FVector2D TextureSize = FVector2D(128, 128);

	UPROPERTY()
		FVector2D TextureOffset = FVector2D(0, 0);

	UPROPERTY(ReplicatedUsing = OnRep_IsEnabled)
		int IsEnabled = -1;

	UPROPERTY(Replicated)
		float OutsideSafetyLevel = 1;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ABRSafeZoneActor> SafeZoneActorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector4> WAIT_WARNING_DURATION_DAMAGE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FVector> SAFECOUNT_SIZESTART_SIZEEND;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString BlowoutNightWeatherName = "blowout_night";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString BlowoutDayWeatherName = "blowout_day";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BlowoutPrepareScale = 0.15;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void StartNextBlowoutStage();

	UFUNCTION()
		void WarnAboutBlowout();

	UFUNCTION()
		void StartDealingDamage();

	UFUNCTION()
		void DamagePlayerOutside();

	UFUNCTION()
		void OnRep_IsEnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		void GetSafeZonesCoords(TArray<FVector> &SafeZonesCoords);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetOutsideSafetyLevel();

	UFUNCTION()
		bool NewSafeZoneOverlapsOld(FVector NewLocation, float Size, TArray<FVector> &SpawnedZones);

	UFUNCTION()
		void LinetraceZ(FVector &SpawnLocation, float BottomZ);


public:

	UFUNCTION()
		void SetEnabled(bool Enabled);

	UFUNCTION()
		void Reset();

	UFUNCTION()
		void Start(FTimecode MatchStartTime, FTimecode WeatherTimePass);

	UFUNCTION()
		void Stop();

	UFUNCTION()
		int GetTimeTillTheEnd();



};
