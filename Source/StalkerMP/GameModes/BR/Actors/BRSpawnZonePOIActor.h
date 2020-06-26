// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/Map/MapPOIActor.h"

#include "BRSpawnZonePOIActor.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API ABRSpawnZonePOIActor : public AMapPOIActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABRSpawnZonePOIActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

public:

	UPROPERTY()
		int TotalSpawners = 0;

	UPROPERTY()
		int TotalItemsWeight = 0;


};
