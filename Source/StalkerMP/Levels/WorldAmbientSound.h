// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Levels/BaseWorldAmbientEffect.h"

#include "WorldAmbientSound.generated.h"

class UAudioComponent;

/**
 * 
 */
UCLASS()
class STALKERMP_API AWorldAmbientSound : public ABaseWorldAmbientEffect
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	AWorldAmbientSound();
	virtual void BeginPlay() override;
	virtual void Update(float NewVolume, float FadeTime);



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		float Volume = 0;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAudioComponent* AudioComponent;


};
