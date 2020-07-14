// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/GameMenu/BaseMenuUI.h"

#include "GeneralSettingsUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UGeneralSettingsUI : public UBaseMenuUI
{
	GENERATED_BODY()





// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	void NativeConstruct() override;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		float NormalizeCrosshairLinesGap(float Gap);

	UFUNCTION()
		float NormalizeCrosshairLinesLength(float Length);

	UFUNCTION()
		float NormalizeCrosshairLinesThickness(float Thickness);

	UFUNCTION()
		float NormalizeCrosshairInaccuracyScale(float Scale);

	UFUNCTION()
		int NormalizeMinimapSize(int Size);

	UFUNCTION()
		float NormalizeMinimapZoom(float Zoom);

	UFUNCTION()
		float NormalizeMinimapIconsScale(float Scale);

	UFUNCTION()
		float NormalizeMapIconsScale(float Scale);



protected:

	UFUNCTION(BlueprintCallable)
		void SetCrosshairLinesGap(float Gap);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnCrosshairLinesGapUpdated(float NewGap);

	UFUNCTION(BlueprintCallable)
		void SetCrosshairLinesLength(float Length);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnCrosshairLinesLengthUpdated(float NewLength);

	UFUNCTION(BlueprintCallable)
		void SetCrosshairLinesThickness(float Thickness);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnCrosshairLinesThicknessUpdated(float NewThickness);

	UFUNCTION(BlueprintCallable)
		void SetCrosshairInaccuracyScale(float Scale);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnCrosshairInaccuracyScaleUpdated(float NewScale);

	UFUNCTION(BlueprintCallable)
		void SetMinimapSize(int Size);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnMinimapSizeUpdated(int NewSize);

	UFUNCTION(BlueprintCallable)
		void SetMinimapZoom(float Zoom);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnMinimapZoomUpdated(float NewZoom);

	UFUNCTION(BlueprintCallable)
		void SetMinimapIconsScale(float Scale);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnMinimapIconsScaleUpdated(float NewScale);

	UFUNCTION(BlueprintCallable)
		void SetMapIconsScale(float Scale);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnMapIconsScaleUpdated(float NewScale);

	UFUNCTION(BlueprintCallable)
		void SetHideUI(bool HideUI);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnHideUIUpdated(bool HideUI);


};
