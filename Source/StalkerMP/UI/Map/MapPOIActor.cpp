// Fill out your copyright notice in the Description page of Project Settings.

#include "MapPOIActor.h"

#include "Components/BoxComponent.h"

// Sets default values
AMapPOIActor::AMapPOIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	POIBox = CreateDefaultSubobject<UBoxComponent>(TEXT("POIBox"));
	POIBox->SetMobility(EComponentMobility::Static);
	POIBox->SetBoxExtent(FVector(100, 100, 100), false);
	POIBox->SetCollisionProfileName(TEXT("NoCollision"));
	POIBox->CanCharacterStepUpOn = ECB_No;
	POIBox->SetGenerateOverlapEvents(false);
	POIBox->SetCanEverAffectNavigation(false);
	POIBox->SetEnableGravity(false);
	POIBox->bApplyImpulseOnDamage = false;
	POIBox->SetCanEverAffectNavigation(false);
	POIBox->AreaClass = nullptr;

#if WITH_EDITORONLY_DATA
	POIBox->bVisualizeComponent = true;
#endif
}

FVector AMapPOIActor::GetScaledBoxSize()
{
	return POIBox->GetScaledBoxExtent();
}
