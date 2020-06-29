// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldAmbientEffectManager.h"

#include "StalkerMP/Levels/WeatherActor.h"
#include "StalkerMP/Levels/BaseWorldAmbientEffect.h"

#include "EngineUtils.h"

AWorldAmbientEffectManager::AWorldAmbientEffectManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTickInterval(1);
	SetTickGroup(ETickingGroup::TG_DuringPhysics);
	SetReplicates(false);
}

void AWorldAmbientEffectManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ABaseWorldAmbientEffect> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABaseWorldAmbientEffect* WorldAmbientEffect = *ActorItr;

		// Only add server effects to server manager and client effects to client manager
		if ((GetNetMode() < ENetMode::NM_Client) == WorldAmbientEffect->GetIsReplicated())
		{
			for (FString WeatherName : WorldAmbientEffect->RequiredWeathers)
			{
				if (!EffectsMap.Contains(WeatherName))
				{
					EffectsMap.Add(WeatherName, FAmbientEffectsArray());
				}

				EffectsMap[WeatherName].Effects.Add(WorldAmbientEffect);
			}
		}
	}

	EffectsMap.GetKeys(WeatherNames);
}

// TODO Optimize this. Find a better way than using .contains()
void AWorldAmbientEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString CurrentWeatherName = WeatherActor->GetCurrentWeatherName();
	FString NextWeatherName = WeatherActor->GetNextWeatherName();
	int CurrentTime = WeatherActor->GetCurrentTime();
	for (FString WeatherName : WeatherNames)
	{
		for (ABaseWorldAmbientEffect* WorldAmbientEffect : EffectsMap[WeatherName].Effects)
		{
			if (!WorldAmbientEffect->RequiredWeathers.Contains(CurrentWeatherName)
				&& !WorldAmbientEffect->RequiredWeathers.Contains(NextWeatherName))
			{
				WorldAmbientEffect->Update(0, WorldAmbientEffect->DefaultFadeTime);
			}
		}
	}

	if (EffectsMap.Contains(CurrentWeatherName))
	{
		for (ABaseWorldAmbientEffect* WorldAmbientEffect : EffectsMap[CurrentWeatherName].Effects)
		{
			if (CurrentTime >= WorldAmbientEffect->StartTimeInt && CurrentTime <= WorldAmbientEffect->EndTimeInt)
			{
				if (WorldAmbientEffect->AffectedByWeatherLerp)
				{
					if (EffectsMap.Contains(NextWeatherName) && EffectsMap[NextWeatherName].Effects.Contains(WorldAmbientEffect))
					{
						WorldAmbientEffect->Update(1, WorldAmbientEffect->DefaultFadeTime);
					}
					else
					{
						WorldAmbientEffect->Update(1 - WeatherActor->GetWeatherLerpValue(), GetActorTickInterval());
					}
				}
				else
				{
					WorldAmbientEffect->Update(1, WorldAmbientEffect->DefaultFadeTime);
				}
			}
			else
			{
				WorldAmbientEffect->Update(0, WorldAmbientEffect->DefaultFadeTime);
			}
		}
	}

	if (EffectsMap.Contains(NextWeatherName))
	{
		for (ABaseWorldAmbientEffect* WorldAmbientEffect : EffectsMap[NextWeatherName].Effects)
		{
			if (CurrentTime >= WorldAmbientEffect->StartTimeInt && CurrentTime <= WorldAmbientEffect->EndTimeInt &&
				(!EffectsMap.Contains(CurrentWeatherName) || !EffectsMap[CurrentWeatherName].Effects.Contains(WorldAmbientEffect)))
			{
				if (WorldAmbientEffect->AffectedByWeatherLerp)
				{
					WorldAmbientEffect->Update(WeatherActor->GetWeatherLerpValue(), GetActorTickInterval());
				}
				else
				{
					WorldAmbientEffect->Update(1, WorldAmbientEffect->DefaultFadeTime);
				}
			}
		}
	}
}
