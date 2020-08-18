// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Items/BaseStaticMeshItem.h"

#include "MoonlightItem.generated.h"

class ABRSafeZoneActor;

class UPointLightComponent;

/**
 * 
 */
UCLASS()
class STALKERMP_API AMoonlightItem : public ABaseStaticMeshItem
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	virtual void FillItemDetails() override;
	virtual void BeforeUse() override;
	virtual void AfterUse() override;


public:
	// Sets default values for this actor's properties
	AMoonlightItem();



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPointLightComponent* PointLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ABRSafeZoneActor> SafeZoneActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int SafeZoneSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int SafeZoneExpandTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int SafeZoneShrinkTime;


};
