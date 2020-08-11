// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldAmbientSound.h"

#include "Components/AudioComponent.h"

AWorldAmbientSound::AWorldAmbientSound()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
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

void AWorldAmbientSound::Update(float NewVolume, float FadeTime)
{
	if (DontFadeOut)
	{
		if (NewVolume > 0 && !AudioComponent->IsPlaying())
		{
			AudioComponent->SetActive(true);
			Volume = 1;
			AudioComponent->Play();
		}
	}
	else
	{
		if (NewVolume > 0 && !AudioComponent->IsActive())
		{
			AudioComponent->SetActive(true);
			AudioComponent->AdjustVolume(0, 0.01, EAudioFaderCurve::Logarithmic);
		}
		if (NewVolume != Volume)
		{
			Volume = NewVolume;
			AudioComponent->AdjustVolume(FadeTime, Volume, EAudioFaderCurve::Logarithmic);
		}
	}
}
