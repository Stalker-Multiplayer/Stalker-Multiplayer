// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSpawnRegion.h"

#include "StalkerMP/Items/ItemSpawners/ItemSpawner.h"

#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"


// Sets default values
AItemSpawnRegion::AItemSpawnRegion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bNetLoadOnClient = false;
	SetActorHiddenInGame(true);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetMobility(EComponentMobility::Static);
	SpawnBox->SetBoxExtent(FVector(32, 32, 1), false);
	SpawnBox->SetCollisionProfileName(TEXT("NoCollision"));
	SpawnBox->CanCharacterStepUpOn = ECB_No;
	SpawnBox->SetGenerateOverlapEvents(false);
	SpawnBox->SetCanEverAffectNavigation(false);
	SpawnBox->SetEnableGravity(false);
	SpawnBox->bApplyImpulseOnDamage = false;
	SpawnBox->SetCanEverAffectNavigation(false);
	SpawnBox->AreaClass = nullptr;

#if WITH_EDITORONLY_DATA
	SpawnBox->bVisualizeComponent = true;
#endif

	RootComponent = SpawnBox;
}

FVector AItemSpawnRegion::GetSpawnLocation()
{
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), SpawnBox->GetScaledBoxExtent());
	//SpawnLocation -= GetActorLocation();
	//SpawnLocation = UKismetMathLibrary::RotateAngleAxis(SpawnLocation, GetActorRotation().Yaw, FVector(0, 1, 0));
	//SpawnLocation += GetActorLocation();
	return SpawnLocation;
}

