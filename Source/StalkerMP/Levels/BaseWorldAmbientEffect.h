// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Misc/Timecode.h"

#include "BaseWorldAmbientEffect.generated.h"

UCLASS()
class STALKERMP_API ABaseWorldAmbientEffect : public AActor
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	ABaseWorldAmbientEffect();
	virtual void BeginPlay() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

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
		TArray<FString> RequiredWeathers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefaultFadeTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AffectedByWeatherLerp = true;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		virtual void Update(float NewWeatherLerp, float FadeTime);


};
