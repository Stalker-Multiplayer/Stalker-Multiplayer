// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/Map/MapWidget.h"

#include "BRMapWidget.generated.h"

class ABRZoneActor;
class UImage;
class UMaterialParameterCollection;

/**
 * 
 */
UCLASS()
class STALKERMP_API UBRMapWidget : public UMapWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Override --------------------------------------------------------------------------

public:

	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		ABRZoneActor* ZoneActor;

	UPROPERTY()
		UMaterialParameterCollection* MapMaterialParameterCollection;



};
