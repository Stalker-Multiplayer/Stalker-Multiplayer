// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoSettingsUI.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"


// ------------------------------------------------------------------------- Screen Settings --------------------------------------------------------------------------

void UVideoSettingsUI::NativeConstruct()
{
	GameUserSettings = GetGameInstance<UStalkerMPGameInstance>()->GetEngine()->GetGameUserSettings();

	TArray<int> FullscreenOptions;
	FullscreenOptions.Add(0);
	FullscreenOptions.Add(1);
	FullscreenOptions.Add(2);
	OnFullscreenModeOptionsUpdated(FullscreenOptions);

	SetFullscreenMode(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::FULLSCREEN_MODE_SETTING_KEY,
		ABasePlayerController::DEFAULT_FULLSCREEN_MODE));

	FString ResolutionString = GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(
		ESettingsType::Video,
		ABasePlayerController::SCREEN_RESOLUTION_SETTING_KEY,
		ABasePlayerController::DEFAULT_SCREEN_RESOLUTION);

	FIntPoint Resolution;
	if (ResolutionString.Equals(ABasePlayerController::DEFAULT_SCREEN_RESOLUTION))
	{
		Resolution = FIntPoint(1024, 768);
	}
	else
	{
		FString Width;
		FString Height;
		ResolutionString.Split("x", &Width, &Height);
		Resolution = FIntPoint(FCString::Atoi(*Width), FCString::Atoi(*Height));
	}
	SetScreenResolution(Resolution);

	ApplyScreenSettings();

	SetScreenPercentage(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::SCREEN_PERCENTAGE_SETTING_KEY,
		ABasePlayerController::DEFAULT_SCREEN_PERCENTAGE));

	SetBrightness(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::Video,
		ABasePlayerController::BRIGHTNESS_SETTING_KEY,
		ABasePlayerController::DEFAULT_BRIGHTNESS));

	SetFieldOfView(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::FIELD_OF_VIEW_SETTING_KEY,
		ABasePlayerController::DEFAULT_FIELD_OF_VIEW));

	FillQualityOptions();

	SetAntialiasingMethod(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::ANTIALIASING_METHOD_SETTING_KEY,
		ABasePlayerController::DEFAULT_ANTIALIASING_METHOD));

	SetAntialiasingQuality(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::ANTIALIASING_QUALITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_ANTIALIASING_QUALITY));

	SetEffectsQuality(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::EFFECTS_QUALITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_EFFECTS_QUALITY));

	SetPostProcessQuality(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::POST_PROCESS_QUALITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_POST_PROCESS_QUALITY));

	SetShadowQuality(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::SHADOW_QUALITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_SHADOW_QUALITY));

	SetTextureQuality(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::TEXTURE_QUALITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_TEXTURE_QUALITY));

	SetAmbientOcclusion(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::AMBIENT_OCCLUSION_SETTING_KEY,
		ABasePlayerController::DEFAULT_AMBIENT_OCCLUSION) > 0);

	SetBloom(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::BLOOM_SETTING_KEY,
		ABasePlayerController::DEFAULT_BLOOM) > 0);

	SetParallax(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::PARALLAX_SETTING_KEY,
		ABasePlayerController::DEFAULT_PARALLAX));

	SetFoliageDensity(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::FOLIAGE_DENSITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_FOLIAGE_DENSITY));

	SetFoliageDistance(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::FOLIAGE_DISTANCE_SETTING_KEY,
		ABasePlayerController::DEFAULT_FOLIAGE_DISTANCE));

	SetShowFPS(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::Video,
		ABasePlayerController::SHOW_FPS_SETTING_KEY,
		ABasePlayerController::DEFAULT_SHOW_FPS) > 0);
}

void UVideoSettingsUI::SetFullscreenMode(int WindowMode)
{
	OnFullscreenModeValueUpdated(WindowMode);
	UpdateScreenResolutionOptions(WindowMode);

	CurrentWindowMode = WindowMode;
}

void UVideoSettingsUI::UpdateScreenResolutionOptions(int CurrentFullscreenMode)
{
	TArray<FIntPoint> ResolutionOptions;
	switch (CurrentFullscreenMode)
	{
	case 0: // fullscreen
		UKismetSystemLibrary::GetSupportedFullscreenResolutions(ResolutionOptions);
		break;
	case 1: // windowed_fullscreen
		break;
	case 2: // windowed
		UKismetSystemLibrary::GetConvenientWindowedResolutions(ResolutionOptions);
		break;
	}
	OnScreenResolutionOptionsUpdated(ResolutionOptions);
}

void UVideoSettingsUI::SetScreenResolution(FIntPoint Resolution)
{
	CurrentResolution = Resolution;
}

void UVideoSettingsUI::ApplyScreenSettings()
{
	if (!GetWorld()->IsPlayInEditor())
	{
		GameUserSettings->RequestResolutionChange(CurrentResolution.X, CurrentResolution.Y, EWindowMode::ConvertIntToWindowMode(CurrentWindowMode), true);
		GameUserSettings->ConfirmVideoMode();
		GameUserSettings->SetFullscreenMode(GameUserSettings->GetLastConfirmedFullscreenMode());
		GameUserSettings->SaveSettings();
	}

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::FULLSCREEN_MODE_SETTING_KEY, CurrentWindowMode);
	GetGameInstance<UStalkerMPGameInstance>()->PutStringSettingValue(ESettingsType::Video, ABasePlayerController::SCREEN_RESOLUTION_SETTING_KEY,
		FString::SanitizeFloat(CurrentResolution.X) + "x" + FString::SanitizeFloat(CurrentResolution.Y));
}




// ------------------------------------------------------------------------- Advanced Settings --------------------------------------------------------------------------

int UVideoSettingsUI::ClampIntValue(int Value, int Min, int Max)
{
	if (Value < Min)
	{
		Value = Min;
	}
	else if (Value > Max)
	{
		Value = Max;
	}

	return Value;
}

float UVideoSettingsUI::ClampFloatValue(float Value, float Min, float Max)
{
	if (Value < Min)
	{
		Value = Min;
	}
	else if (Value > Max)
	{
		Value = Max;
	}

	return Value;
}

void UVideoSettingsUI::SetScreenPercentage(int ScreenPercentage)
{
	ScreenPercentage = ClampIntValue(ScreenPercentage, 50, 100);
	OnScreenPercentageUpdated(ScreenPercentage);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::SCREEN_PERCENTAGE_SETTING_KEY, ScreenPercentage);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetBrightness(float Brightness)
{
	Brightness = ClampFloatValue(Brightness, 0.5f, 5.0f);
	OnBrightnessUpdated(Brightness);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::Video, ABasePlayerController::BRIGHTNESS_SETTING_KEY, Brightness);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetFieldOfView(int FieldOfView)
{
	FieldOfView = ClampIntValue(FieldOfView, 60, 120);
	OnFieldOfViewUpdated(FieldOfView);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::FIELD_OF_VIEW_SETTING_KEY, FieldOfView);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetAntialiasingMethod(int AntialiasingMethod)
{
	AntialiasingMethod = ClampIntValue(AntialiasingMethod, 0, 2);
	OnAntialiasingMethodUpdated(AntialiasingMethod);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::ANTIALIASING_METHOD_SETTING_KEY, AntialiasingMethod);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetAntialiasingQuality(int AntialiasingQuality)
{
	AntialiasingQuality = ClampIntValue(AntialiasingQuality, 1, 4);
	OnAntialiasingQualityUpdated(AntialiasingQuality);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::ANTIALIASING_QUALITY_SETTING_KEY, AntialiasingQuality);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetEffectsQuality(int EffectsQuality)
{
	EffectsQuality = ClampIntValue(EffectsQuality, 0, 4);
	OnEffectsQualityUpdated(EffectsQuality);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::EFFECTS_QUALITY_SETTING_KEY, EffectsQuality);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetPostProcessQuality(int PostProcessQuality)
{
	PostProcessQuality = ClampIntValue(PostProcessQuality, 0, 4);
	OnPostProcessQualityUpdated(PostProcessQuality);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::POST_PROCESS_QUALITY_SETTING_KEY, PostProcessQuality);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetShadowQuality(int ShadowQuality)
{
	ShadowQuality = ClampIntValue(ShadowQuality, 0, 4);
	OnShadowQualityUpdated(ShadowQuality);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::SHADOW_QUALITY_SETTING_KEY, ShadowQuality);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetTextureQuality(int TextureQuality)
{
	TextureQuality = ClampIntValue(TextureQuality, 0, 4);
	OnTextureQualityUpdated(TextureQuality);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::TEXTURE_QUALITY_SETTING_KEY, TextureQuality);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetAmbientOcclusion(bool AmbientOcclusion)
{
	OnAmbientOcclusionUpdated(AmbientOcclusion);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::AMBIENT_OCCLUSION_SETTING_KEY, AmbientOcclusion);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetBloom(bool Bloom)
{
	OnBloomUpdated(Bloom);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::BLOOM_SETTING_KEY, Bloom);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetParallax(bool Parallax)
{
	OnParallaxUpdated(Parallax);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::PARALLAX_SETTING_KEY, Parallax);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetFoliageDensity(int FoliageDensity)
{
	FoliageDensity = ClampIntValue(FoliageDensity, 0, 4);
	OnFoliageDensityUpdated(FoliageDensity);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::FOLIAGE_DENSITY_SETTING_KEY, FoliageDensity);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetFoliageDistance(int FoliageDistance)
{
	FoliageDistance = ClampIntValue(FoliageDistance, 0, 4);
	OnFoliageDistanceUpdated(FoliageDistance);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::FOLIAGE_DISTANCE_SETTING_KEY, FoliageDistance);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}

void UVideoSettingsUI::SetShowFPS(bool ShowFPS)
{
	OnShowFPSUpdated(ShowFPS);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::Video, ABasePlayerController::SHOW_FPS_SETTING_KEY, ShowFPS ? 1 : 0);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Video);
}