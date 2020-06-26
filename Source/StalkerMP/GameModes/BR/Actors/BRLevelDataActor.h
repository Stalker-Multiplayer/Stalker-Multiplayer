// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StalkerMP/DataClasses/BaseLevelDataActor.h"
#include "BRLevelDataActor.generated.h"

class ABRZoneActor;
class UMaterialParameterCollection;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABRLevelDataActor : public ABaseLevelDataActor
{
	GENERATED_BODY()
	


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BR")
		ABRZoneActor* ZoneActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		UMaterialParameterCollection* MapMaterialParameterCollection;


};
