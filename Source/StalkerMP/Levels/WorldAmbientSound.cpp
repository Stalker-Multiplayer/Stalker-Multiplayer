// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldAmbientSound.h"

#include "StalkerMP/SMPFunctions.h"

#include "Components/AudioComponent.h"

AWorldAmbientSound::AWorldAmbientSound()
{
	PrimaryActorTick.bCanEverTick = false;
	//SetActorHiddenInGame(true);
	SetReplicates(false);
	GetAudioComponent()->bAutoActivate = false;
}

void AWorldAmbientSound::BeginPlay()
{
	Super::BeginPlay();

	GetAudioComponent()->AdjustVolume(0.01, 0, EAudioFaderCurve::Logarithmic);

	StartTimeInt = USMPFunctions::TimecodeToSeconds(StartTime);
	EndTimeInt = USMPFunctions::TimecodeToSeconds(EndTime);
}

void AWorldAmbientSound::SetVolume(float NewVolume, float FadeTime)
{
	if (NewVolume > 0 && !GetAudioComponent()->IsActive())
	{
		GetAudioComponent()->SetActive(true);
	}
	if (NewVolume != Volume)
	{
		Volume = NewVolume;
		GetAudioComponent()->AdjustVolume(FadeTime, Volume, EAudioFaderCurve::Logarithmic);
	}
}
