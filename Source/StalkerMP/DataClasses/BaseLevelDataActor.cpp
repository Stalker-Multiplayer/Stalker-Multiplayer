// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseLevelDataActor.h"

#include "StalkerMP/StalkerMPGameInstance.h"

#include "Components/SphereComponent.h"

FGamemodeLevelLoadedDelegate ABaseLevelDataActor::GamemodeLevelLoadedDelegate = FGamemodeLevelLoadedDelegate();

// Sets default values
ABaseLevelDataActor::ABaseLevelDataActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	EmptySphere = CreateDefaultSubobject<USphereComponent>(TEXT("EmptySphere"));
	EmptySphere->SetCollisionProfileName(TEXT("NoCollision"));
	EmptySphere->SetMobility(EComponentMobility::Static);
	EmptySphere->SetSphereRadius(0);
	EmptySphere->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	EmptySphere->bVisualizeComponent = true;
#endif
}

void ABaseLevelDataActor::BeginPlay()
{
	Super::BeginPlay();

	GamemodeLevelLoadedDelegate.Broadcast();
}
