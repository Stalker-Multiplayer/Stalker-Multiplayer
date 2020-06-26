// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapPOIWidget.generated.h"

class AMapPOIActor;

class UImage;
class UWidget;

/**
 * 
 */
UCLASS()
class STALKERMP_API UMapPOIWidget : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		AMapPOIActor* MapPOIActor;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void SetPOIActor(AMapPOIActor* TheMapPOIActor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetImageWidget();

	UFUNCTION()
		FVector GetPOILocation();

	UFUNCTION()
		float GetPOIRotationYaw();

	UFUNCTION()
		FVector GetPOISize();

};
