// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "MapWidget.generated.h"

class ABasePlayerController;
class ABaseLevelDataActor;
class UMapPOIWidget;

class UMaterialInterface;
class UOverlay;
class UImage;

/**
 * 
 */
UCLASS()
class STALKERMP_API UMapWidget : public UHideableWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Override --------------------------------------------------------------------------

public:

	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnFloatSettingValueUpdatedDelegate;

	UPROPERTY()
		TArray<UMapPOIWidget*> POIWidgets;

	UPROPERTY()
		FTimerHandle CreatePOIWidgetsTimerHandle;


protected:

	UPROPERTY()
		float Zoom = 1;

	UPROPERTY()
		float CmPerPixel = 100;

	UPROPERTY()
		FVector2D TextureSize = FVector2D(128, 128);

	UPROPERTY()
		FVector2D TextureOffset = FVector2D(0, 0);

	UPROPERTY()
		FVector2D Offset = FVector2D(0, 0);

	UPROPERTY()
		ABasePlayerController* PlayerController;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnFloatSettingUpdated(FString Key, float Value);

	UFUNCTION()
		void CreatePOIWidgets();


protected:

	UFUNCTION()
		virtual void SetMaterial(UMaterialInterface* MaterialInterface, FVector2D TheTextureSize);

	UFUNCTION()
		virtual void SetZoom(float TheZoom);

	UFUNCTION()
		virtual void SetOffset(FVector2D TheOffset);

	UFUNCTION()
		virtual void SetIconsScale(float TheScale);

	UFUNCTION()
		FVector2D WorldCoordsToMapCoords(float X, float Y);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UOverlay* GetMapOverlay();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UOverlay* GetPOIWidgetsOverlay();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetMapImage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetPlayerImage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetPlayerLookDirectionImage();


public:

	UFUNCTION(BlueprintCallable)
		void SetPlayerController(ABasePlayerController* ThePlayerController) { PlayerController = ThePlayerController; }

	UFUNCTION()
		void ZoomIn();

	UFUNCTION()
		void ZoomOut();


};
