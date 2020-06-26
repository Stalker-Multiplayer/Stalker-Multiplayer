// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseSkeletalMeshItem.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"


ABaseSkeletalMeshItem::ABaseSkeletalMeshItem()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	SkeletalMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("PickableItem"));
	SkeletalMeshComponent->bComponentUseFixedSkelBounds = true;
	SkeletalMeshComponent->CanCharacterStepUpOn = ECB_No;
	SkeletalMeshComponent->SetGenerateOverlapEvents(false);
	SkeletalMeshComponent->SetCanEverAffectNavigation(false);
	SkeletalMeshComponent->bCastStaticShadow = false;
	SkeletalMeshComponent->bPerBoneMotionBlur = false;
	//SkeletalMeshComponent->SetSimulatePhysics(true);
	RootComponent = SkeletalMeshComponent;

	PickupCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollision"));
	PickupCollision->PrimaryComponentTick.bCanEverTick = false;
	PickupCollision->PrimaryComponentTick.bStartWithTickEnabled = false;
	PickupCollision->SetCollisionProfileName(TEXT("ActionReceiver"));
	PickupCollision->CanCharacterStepUpOn = ECB_No;
	PickupCollision->SetGenerateOverlapEvents(true);
	PickupCollision->SetCanEverAffectNavigation(false);
	PickupCollision->SetupAttachment(RootComponent);
}

void ABaseSkeletalMeshItem::OnRep_CanBePickedUp()
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
