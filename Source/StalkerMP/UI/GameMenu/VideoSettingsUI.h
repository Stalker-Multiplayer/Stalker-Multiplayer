// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/GameMenu/BaseMenuUI.h"

#include "VideoSettingsUI.generated.h"

class UGameUserSettings;

/**
 * 
 */
UCLASS()
class STALKERMP_API UVideoSettingsUI : public UBaseMenuUI
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	void NativeConstruct() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UGameUserSettings* GameUserSettings;

	UPROPERTY()
		int CurrentWindowMode;

	UPROPERTY()
		FIntPoint CurrentResolution;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	// Screen Settings

	UFUNCTION(BlueprintCallable)
		void SetFullscreenMode(int WindowMode); // 0 - fullscreen, 1 - windowed_fullscreen, 2 - windowed

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnFullscreenModeOptionsUpdated(const TArray<int>& WindowModes); // 0 - fullscreen, 1 - windowed_fullscreen, 2 - windowed

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnFullscreenModeValueUpdated(int WindowMode); // 0 - fullscreen, 1 - windowed_fullscreen, 2 - windowed

	UFUNCTION(BlueprintCallable)
		void SetScreenResolution(FIntPoint Resolution);

	UFUNCTION()
		void UpdateScreenResolutionOptions(int CurrentFullscreenMode);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnScreenResolutionOptionsUpdated(const TArray<FIntPoint>& Resolutions);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnScreenResolutionValueUpdated(FIntPoint Resolution);

	UFUNCTION(BlueprintCallable)
		void ApplyScreenSettings();



	// Advanced Settings

	UFUNCTION()
		int ClampIntValue(int Value, int Min, int Max);

	UFUNCTION()
		float ClampFloatValue(float Value, float Min, float Max);

	UFUNCTION(BlueprintCallable)
		void SetScreenPercentage(int ScreenPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnScreenPercentageUpdated(int ScreenPercentage);

	UFUNCTION(BlueprintCallable)
		void SetBrightness(float Brightness);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnBrightnessUpdated(float Brightness);

	UFUNCTION(BlueprintCallable)
		void SetFieldOfView(int FieldOfView);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnFieldOfViewUpdated(int FieldOfView);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void FillQualityOptions();

	UFUNCTION(BlueprintCallable)
		void SetAntialiasingMethod(int AntialiasingMethod);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnAntialiasingMethodUpdated(int AntialiasingMethod);

	UFUNCTION(BlueprintCallable)
		void SetAntialiasingQuality(int AntialiasingQuality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnAntialiasingQualityUpdated(int AntialiasingQuality);

	UFUNCTION(BlueprintCallable)
		void SetEffectsQuality(int EffectsQuality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnEffectsQualityUpdated(int EffectsQuality);

	UFUNCTION(BlueprintCallable)
		void SetPostProcessQuality(int PostProcessQuality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnPostProcessQualityUpdated(int PostProcessQuality);

	UFUNCTION(BlueprintCallable)
		void SetShadowQuality(int ShadowQuality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnShadowQualityUpdated(int ShadowQuality);

	UFUNCTION(BlueprintCallable)
		void SetTextureQuality(int TextureQuality);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnTextureQualityUpdated(int TextureQuality);

	UFUNCTION(BlueprintCallable)
		void SetAmbientOcclusion(bool AmbientOcclusion);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnAmbientOcclusionUpdated(bool AmbientOcclusion);

	UFUNCTION(BlueprintCallable)
		void SetBloom(bool Bloom);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnBloomUpdated(bool Bloom);

	UFUNCTION(BlueprintCallable)
		void SetParallax(bool Parallax);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnParallaxUpdated(bool Parallax);

	UFUNCTION(BlueprintCallable)
		void SetFoliageDensity(int FoliageDensity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnFoliageDensityUpdated(int FoliageDensity);

	UFUNCTION(BlueprintCallable)
		void SetFoliageDistance(int FoliageDistance);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnFoliageDistanceUpdated(int FoliageDistance);

	UFUNCTION(BlueprintCallable)
		void SetShowFPS(bool ShowFPS);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnShowFPSUpdated(bool ShowFPS);

};
