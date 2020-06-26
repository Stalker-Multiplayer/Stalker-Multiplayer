// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldAmbientSoundManager.h"

#include "StalkerMP/Levels/WeatherActor.h"
#include "StalkerMP/Levels/WorldAmbientSound.h"

#include "EngineUtils.h"

AWorldAmbientSoundManager::AWorldAmbientSoundManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(1);
	bAllowReceiveTickEventOnDedicatedServer = false;
	SetTickGroup(ETickingGroup::TG_DuringPhysics);
	//SetActorHiddenInGame(true);
	SetReplicates(false);
}

void AWorldAmbientSoundManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AWorldAmbientSound> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AWorldAmbientSound* WorldAmbientSound = *ActorItr;

		for (FString WeatherName : WorldAmbientSound->RequiredWeathers)
		{
			if (!SoundsMap.Contains(WeatherName))
			{
				SoundsMap.Add(WeatherName, FAmbientSoundsArray());
			}

			SoundsMap[WeatherName].Sounds.Add(WorldAmbientSound);
		}
	}

	SoundsMap.GetKeys(WeatherNames);
}

// TODO Optimize this. Find a better way than using .contains()
void AWorldAmbientSoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString CurrentWeatherName = WeatherActor->GetCurrentWeatherName();
	FString NextWeatherName = WeatherActor->GetNextWeatherName();
	int CurrentTime = WeatherActor->GetCurrentTime();
	for (FString WeatherName : WeatherNames)
	{
		for (AWorldAmbientSound* WorldAmbientSound : SoundsMap[WeatherName].Sounds)
		{
			if (!WorldAmbientSound->RequiredWeathers.Contains(CurrentWeatherName)
				&& !WorldAmbientSound->RequiredWeathers.Contains(NextWeatherName))
			{
				WorldAmbientSound->SetVolume(0, WorldAmbientSound->DefaultFadeTime);
			}
		}
	}

	if (SoundsMap.Contains(CurrentWeatherName))
	{
		for (AWorldAmbientSound* WorldAmbientSound : SoundsMap[CurrentWeatherName].Sounds)
		{
			if (CurrentTime >= WorldAmbientSound->StartTimeInt && CurrentTime <= WorldAmbientSound->EndTimeInt)
			{
				if (WorldAmbientSound->AffectedByWeatherLerp)
				{
					if (SoundsMap.Contains(NextWeatherName) && SoundsMap[NextWeatherName].Sounds.Contains(WorldAmbientSound))
					{
						WorldAmbientSound->SetVolume(1, WorldAmbientSound->DefaultFadeTime);
					}
					else
					{
						WorldAmbientSound->SetVolume(1 - WeatherActor->GetWeatherLerpValue(), GetActorTickInterval());
					}
				}
				else
				{
					WorldAmbientSound->SetVolume(1, WorldAmbientSound->DefaultFadeTime);
				}
			}
			else
			{
				WorldAmbientSound->SetVolume(0, WorldAmbientSound->DefaultFadeTime);
			}
		}
	}

	if (SoundsMap.Contains(NextWeatherName))
	{
		for (AWorldAmbientSound* WorldAmbientSound : SoundsMap[NextWeatherName].Sounds)
		{
			if (CurrentTime >= WorldAmbientSound->StartTimeInt && CurrentTime <= WorldAmbientSound->EndTimeInt &&
				(!SoundsMap.Contains(CurrentWeatherName) || !SoundsMap[CurrentWeatherName].Sounds.Contains(WorldAmbientSound)))
			{
				if (WorldAmbientSound->AffectedByWeatherLerp)
				{
					WorldAmbientSound->SetVolume(WeatherActor->GetWeatherLerpValue(), GetActorTickInterval());
				}
				else
				{
					WorldAmbientSound->SetVolume(1, WorldAmbientSound->DefaultFadeTime);
				}
			}
		}
	}
}
