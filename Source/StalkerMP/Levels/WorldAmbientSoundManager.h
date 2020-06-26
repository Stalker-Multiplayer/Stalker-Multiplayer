// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "WorldAmbientSoundManager.generated.h"

class AWeatherActor;
class AWorldAmbientSound;

USTRUCT()
struct FAmbientSoundsArray
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<AWorldAmbientSound*> Sounds;

};

UCLASS()
class STALKERMP_API AWorldAmbientSoundManager : public AActor
{
	GENERATED_BODY()
	

// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	AWorldAmbientSoundManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		AWeatherActor* WeatherActor;

	UPROPERTY()
		TMap<FString, FAmbientSoundsArray> SoundsMap;

	UPROPERTY()
		TArray<FString> WeatherNames;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void SetWeatherActor(AWeatherActor* TheWeatherActor) { WeatherActor = TheWeatherActor; }


};
