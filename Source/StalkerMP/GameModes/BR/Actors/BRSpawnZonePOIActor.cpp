// Fill out your copyright notice in the Description page of Project Settings.

#include "BRSpawnZonePOIActor.h"

#include "StalkerMP/Items/ItemSpawners/ItemSpawner.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"

ABRSpawnZonePOIActor::ABRSpawnZonePOIActor()
{
	MatchBoxSize = false;
	RotateAccordingToBox = false;
}

void ABRSpawnZonePOIActor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AItemSpawner> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AItemSpawner* ItemSpawner = *ActorItr;

		if (UKismetMathLibrary::IsPointInBoxWithTransform(ItemSpawner->GetActorLocation(), GetTransform(), POIBox->GetScaledBoxExtent()))
		{
			TotalSpawners++;
			TotalItemsWeight += ItemSpawner->GetTotalItemsWeight();
		}
	}

	FFormatNamedArguments Args;
	Args.Add("TotalSpawners", TotalSpawners);
	Args.Add("TotalItemsWeight", TotalItemsWeight);
	TooltipText = FText::Format(TooltipText, Args);
}
