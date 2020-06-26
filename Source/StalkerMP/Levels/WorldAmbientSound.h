// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Misc/Timecode.h"
#include "Sound/AmbientSound.h"

#include "WorldAmbientSound.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API AWorldAmbientSound : public AAmbientSound
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	AWorldAmbientSound();
	virtual void BeginPlay() override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		float Volume = 0;


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTimecode StartTime = FTimecode(0, 0, 0, 0, false);

	UPROPERTY()
		int StartTimeInt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTimecode EndTime = FTimecode(24, 0, 0, 0, false);

	UPROPERTY()
		int EndTimeInt = 86400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PlayedOnlyOutside = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AffectedByWeatherLerp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> RequiredWeathers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefaultFadeTime = 1.0f;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void SetVolume(float NewVolume, float FadeTime);


};
