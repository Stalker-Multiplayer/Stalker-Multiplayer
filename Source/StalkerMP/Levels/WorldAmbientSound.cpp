// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldAmbientSound.h"

#include "Components/AudioComponent.h"

AWorldAmbientSound::AWorldAmbientSound()
{
	PrimaryActorTick.bCanEverTick = false;
	SetHidden(true);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bStopWhenOwnerDestroyed = true;
	AudioComponent->bShouldRemainActiveIfDropped = true;
	AudioComponent->Mobility = EComponentMobility::Static;
	RootComponent = AudioComponent;
}

void AWorldAmbientSound::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->AdjustVolume(0.01, 0, EAudioFaderCurve::Logarithmic);
}

void AWorldAmbientSound::Update(float NewWeatherLerp, float FadeTime)
{
	if (NewWeatherLerp > 0 && !AudioComponent->IsActive())
	{
		AudioComponent->SetActive(true);
	}
	if (NewWeatherLerp != Volume)
	{
		Volume = NewWeatherLerp;
		AudioComponent->AdjustVolume(FadeTime, Volume, EAudioFaderCurve::Logarithmic);
	}
}
