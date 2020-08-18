// Fill out your copyright notice in the Description page of Project Settings.


#include "MoonlightItem.h"

#include "StalkerMP/GameModes/BR/Actors/BRSafeZoneActor.h"

#include "Components/PointLightComponent.h"
#include "EngineUtils.h"


AMoonlightItem::AMoonlightItem()
{
	CanBeUsed = true;

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLightComponent->PrimaryComponentTick.bCanEverTick = false;
	PointLightComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	PointLightComponent->Intensity = 500;
	PointLightComponent->AttenuationRadius = 50;
	PointLightComponent->LightColor = FColor(0, 31, 127);
	PointLightComponent->MaxDrawDistance = 3000;
	PointLightComponent->MaxDistanceFadeRange = 2000;
	PointLightComponent->SetupAttachment(RootComponent);
}

void AMoonlightItem::FillItemDetails()
{
	Super::FillItemDetails();

	FFormatNamedArguments Args;

	Args.Add("SafeZoneSize", SafeZoneSize);
	Args.Add("SecondsLast", SafeZoneShrinkTime);

	ItemDetails = FText::Format(ItemDetails, Args);
}

void AMoonlightItem::BeforeUse()
{

}

void AMoonlightItem::AfterUse()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetOwner()->GetActorLocation());

	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABRSafeZoneActor* SafeZoneActor = GetWorld()->SpawnActor<ABRSafeZoneActor>(SafeZoneActorClass, SpawnTransform, SpawnInfo);
	SafeZoneActor->SetParams(SafeZoneSize, 0, SafeZoneExpandTime, SafeZoneShrinkTime, false);

	Super::AfterUse();
}
