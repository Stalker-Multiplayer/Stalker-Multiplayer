// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Levels/BaseWorldAmbientEffect.h"

#include "WorldAmbientThunder.generated.h"

class UAudioComponent;
class UDirectionalLightComponent;
class USoundBase;

/**
 * 
 */
UCLASS()
class STALKERMP_API AWorldAmbientThunder : public ABaseWorldAmbientEffect
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	AWorldAmbientThunder();
	virtual void Update(float NewWeatherLerp, float FadeTime);



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		FTimerHandle ThunderTimerHandle;

	UPROPERTY()
		float SoundDelay = 0;

	

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USceneComponent* DefaultComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UAudioComponent* AudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UDirectionalLightComponent* LightComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<USoundBase*> ThunderSounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator MinThunderRotation = FRotator(200, 20, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator MaxThunderRotation = FRotator(340, 110, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinThunderDelay = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxThunderDelay = 60;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinSoundDelay = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxSoundDelay = 3;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void SetupNextThunder();

	UFUNCTION()
		void ShowThunder();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_ShowThunder(FRotator LightRotator, int ThunderSoundIndex, float NewSoundDelay);

	UFUNCTION()
		void ShowThunderLight1();

	UFUNCTION()
		void HideThunderLight1();

	UFUNCTION()
		void ShowThunderLight2();

	UFUNCTION()
		void HideThunderLight2();

	UFUNCTION()
		void PlayThunderSound();


};
