// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "WorldAmbientEffectManager.generated.h"

class AWeatherActor;
class ABaseWorldAmbientEffect;

USTRUCT()
struct FAmbientEffectsArray
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<ABaseWorldAmbientEffect*> Effects;

};

UCLASS()
class STALKERMP_API AWorldAmbientEffectManager : public AActor
{
	GENERATED_BODY()
	

// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	AWorldAmbientEffectManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		AWeatherActor* WeatherActor;

	UPROPERTY()
		TMap<FString, FAmbientEffectsArray> EffectsMap;

	UPROPERTY()
		TArray<FString> WeatherNames;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void SetWeatherActor(AWeatherActor* TheWeatherActor) { WeatherActor = TheWeatherActor; }


};
