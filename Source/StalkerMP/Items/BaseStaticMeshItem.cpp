// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseStaticMeshItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"


ABaseStaticMeshItem::ABaseStaticMeshItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->PrimaryComponentTick.bCanEverTick = false;
	StaticMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	StaticMeshComponent->SetCollisionProfileName(TEXT("PickableItem"));
	//StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->bCastStaticShadow = false;
	RootComponent = StaticMeshComponent;

	PickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollision"));
	PickupCollision->PrimaryComponentTick.bCanEverTick = false;
	PickupCollision->PrimaryComponentTick.bStartWithTickEnabled = false;
	PickupCollision->SetCollisionProfileName(TEXT("ActionReceiver"));
	PickupCollision->CanCharacterStepUpOn = ECB_No;
	PickupCollision->SetGenerateOverlapEvents(true);
	PickupCollision->SetCanEverAffectNavigation(false);
	PickupCollision->SetupAttachment(RootComponent);
}

void ABaseStaticMeshItem::OnRep_CanBePickedUp()
{
	Super::OnRep_CanBePickedUp();

	if (CanBePickedUp)
	{
		PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		PickupCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
