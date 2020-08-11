// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWorldAmbientEffect.h"

#include "StalkerMP/SMPFunctions.h"

ABaseWorldAmbientEffect::ABaseWorldAmbientEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetReplicates(false);
	SetCanBeDamaged(false);
}

void ABaseWorldAmbientEffect::BeginPlay()
{
	Super::BeginPlay();

	StartTimeInt = USMPFunctions::TimecodeToSeconds(StartTime);
	EndTimeInt = USMPFunctions::TimecodeToSeconds(EndTime);
}

void ABaseWorldAmbientEffect::Update(float NewWeatherLerp, float FadeTime)
{}
