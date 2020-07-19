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
			for (FString WeatherType : WorldAmbientEffect->RequiredWeathers)
			{
				if (!EffectsMap.Contains(WeatherType))
				{
					EffectsMap.Add(WeatherType, FAmbientEffectsArray());
				}

				EffectsMap[WeatherType].Effects.Add(WorldAmbientEffect);
			}
		}
	}

	EffectsMap.GetKeys(WeatherTypes);
}

// TODO Optimize this. Find a better way than using .contains()
void AWorldAmbientEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString CurrentWeatherType = WeatherActor->GetCurrentWeatherType();
	FString NextWeatherType = WeatherActor->GetNextWeatherType();
	int CurrentTime = WeatherActor->GetCurrentTime();
	for (FString WeatherType : WeatherTypes)
	{
		for (ABaseWorldAmbientEffect* WorldAmbientEffect : EffectsMap[WeatherType].Effects)
		{
			if (!WorldAmbientEffect->RequiredWeathers.Contains(CurrentWeatherType)
				&& !WorldAmbientEffect->RequiredWeathers.Contains(NextWeatherType))
			{
				WorldAmbientEffect->Update(0, WorldAmbientEffect->DefaultFadeTime);
			}
		}
	}

	if (EffectsMap.Contains(CurrentWeatherType))
	{
		for (ABaseWorldAmbientEffect* WorldAmbientEffect : EffectsMap[CurrentWeatherType].Effects)
		{
			if (CurrentTime >= WorldAmbientEffect->StartTimeInt && CurrentTime <= WorldAmbientEffect->EndTimeInt)
			{
				if (WorldAmbientEffect->AffectedByWeatherLerp)
				{
					if (EffectsMap.Contains(NextWeatherType) && EffectsMap[NextWeatherType].Effects.Contains(WorldAmbientEffect))
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

	if (EffectsMap.Contains(NextWeatherType))
	{
		for (ABaseWorldAmbientEffect* WorldAmbientEffect : EffectsMap[NextWeatherType].Effects)
		{
			if (CurrentTime >= WorldAmbientEffect->StartTimeInt && CurrentTime <= WorldAmbientEffect->EndTimeInt &&
				(!EffectsMap.Contains(CurrentWeatherType) || !EffectsMap[CurrentWeatherType].Effects.Contains(WorldAmbientEffect)))
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
