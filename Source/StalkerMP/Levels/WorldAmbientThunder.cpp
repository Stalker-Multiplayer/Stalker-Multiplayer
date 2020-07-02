// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldAmbientThunder.h"

#include "StalkerMP/Levels/WeatherActor.h"

#include "Components/AudioComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/Public/TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"

AWorldAmbientThunder::AWorldAmbientThunder()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	NetCullDistanceSquared = 40000000000.0;

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	DefaultComponent->SetMobility(EComponentMobility::Static);
	RootComponent = DefaultComponent;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bStopWhenOwnerDestroyed = true;
	AudioComponent->bShouldRemainActiveIfDropped = true;
	AudioComponent->Mobility = EComponentMobility::Static;
	AudioComponent->SetupAttachment(RootComponent);

	LightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("LightComponent"));
	LightComponent->SetMobility(EComponentMobility::Movable);
	LightComponent->bEnableLightShaftBloom = true;
	LightComponent->Intensity = 0;
	LightComponent->BloomScale = 0.025;
	LightComponent->BloomMaxBrightness = 10;
	LightComponent->bUseRayTracedDistanceFieldShadows = true;
	LightComponent->DistanceFieldShadowDistance = 200000;
	LightComponent->SetupAttachment(RootComponent);
}

void AWorldAmbientThunder::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AWeatherActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		WeatherActor = *ActorItr;
		break;
	}
}

void AWorldAmbientThunder::Update(float NewWeatherLerp, float FadeTime)
{
	if (NewWeatherLerp > 0.5)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(ThunderTimerHandle))
		{
			SetupNextThunder();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void AWorldAmbientThunder::SetupNextThunder()
{
	int ThunderDelay = UKismetMathLibrary::RandomInteger(MaxThunderDelay - MinThunderDelay) + MinThunderDelay;
	GetWorld()->GetTimerManager().SetTimer(ThunderTimerHandle, this, &AWorldAmbientThunder::ShowThunder, ThunderDelay, false);
}

void AWorldAmbientThunder::ShowThunder()
{
	FRotator LightRotator = FRotator(
		UKismetMathLibrary::RandomInteger(MaxThunderRotation.Pitch - MinThunderRotation.Pitch) + MinThunderRotation.Pitch,
		UKismetMathLibrary::RandomInteger(MaxThunderRotation.Yaw - MinThunderRotation.Yaw) + MinThunderRotation.Yaw,
		UKismetMathLibrary::RandomInteger(MaxThunderRotation.Roll - MinThunderRotation.Roll) + MinThunderRotation.Roll
	);
	int ThunderSound = UKismetMathLibrary::RandomInteger(ThunderSounds.Num());
	SoundDelay = UKismetMathLibrary::RandomFloatInRange(MinSoundDelay, MaxSoundDelay);

	Multicast_ShowThunder(LightRotator, ThunderSound, SoundDelay);
	SetupNextThunder();
}

void AWorldAmbientThunder::Multicast_ShowThunder_Implementation(FRotator LightRotator, int ThunderSoundIndex, float NewSoundDelay)
{
	LightComponent->SetWorldRotation(LightRotator);
	AudioComponent->SetSound(ThunderSounds[ThunderSoundIndex]);
	SoundDelay = NewSoundDelay;
	ShowThunderLight1();
}

void AWorldAmbientThunder::ShowThunderLight1()
{
	LastFogMaxOpacity = WeatherActor->GetFogMaxOpacity();
	LightComponent->SetIntensity(10);
	WeatherActor->SetFogMaxOpacity(0.9f);
	GetWorld()->GetTimerManager().SetTimer(ThunderTimerHandle, this, &AWorldAmbientThunder::HideThunderLight1, 0.05, false);
}

void AWorldAmbientThunder::HideThunderLight1()
{
	LightComponent->SetIntensity(0);
	WeatherActor->SetFogMaxOpacity(LastFogMaxOpacity);
	GetWorld()->GetTimerManager().SetTimer(ThunderTimerHandle, this, &AWorldAmbientThunder::ShowThunderLight2, 0.05, false);
}

void AWorldAmbientThunder::ShowThunderLight2()
{
	LastFogMaxOpacity = WeatherActor->GetFogMaxOpacity();
	LightComponent->SetIntensity(10);
	WeatherActor->SetFogMaxOpacity(0.9f);
	GetWorld()->GetTimerManager().SetTimer(ThunderTimerHandle, this, &AWorldAmbientThunder::HideThunderLight2, 0.05, false);
}

void AWorldAmbientThunder::HideThunderLight2()
{
	LightComponent->SetIntensity(0);
	WeatherActor->SetFogMaxOpacity(LastFogMaxOpacity);
	GetWorld()->GetTimerManager().SetTimer(ThunderTimerHandle, this, &AWorldAmbientThunder::PlayThunderSound, SoundDelay, false, SoundDelay);
}

void AWorldAmbientThunder::PlayThunderSound()
{
	AudioComponent->Play();
}
