// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralSettingsUI.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"


void UGeneralSettingsUI::NativeConstruct()
{
	Super::NativeConstruct();


	// Crosshair

	SetCrosshairLinesGap(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::CROSSHAIR_LINES_GAP_SETTING_KEY,
		ABasePlayerController::DEFAULT_CROSSHAIR_LINES_GAP));
	SetCrosshairLinesLength(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::CROSSHAIR_LINES_LENGTH_SETTING_KEY,
		ABasePlayerController::DEFAULT_CROSSHAIR_LINES_LENGTH));
	SetCrosshairLinesThickness(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::CROSSHAIR_LINES_THICKNESS_SETTING_KEY,
		ABasePlayerController::DEFAULT_CROSSHAIR_LINES_THICKNESS));
	SetCrosshairInaccuracyScale(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::CROSSHAIR_INACCURACY_SCALE_SETTING_KEY,
		ABasePlayerController::DEFAULT_CROSSHAIR_INACCURACY_SCALE));


	// Map and Minimap

	SetMinimapSize(GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::General,
		ABasePlayerController::MINIMAP_SIZE_SETTING_KEY,
		ABasePlayerController::DEFAULT_MINIMAP_SIZE));
	SetMinimapZoom(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::MINIMAP_ZOOM_SETTING_KEY,
		ABasePlayerController::DEFAULT_MINIMAP_ZOOM));
	SetMinimapIconsScale(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::MINIMAP_ICONS_SCALE_SETTING_KEY,
		ABasePlayerController::DEFAULT_MINIMAP_ICONS_SIZE));
	SetMapIconsScale(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::MAP_ICONS_SCALE_SETTING_KEY,
		ABasePlayerController::DEFAULT_MAP_ICONS_SIZE));


	// Other

	SetHideUI((bool) GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
		ESettingsType::General,
		ABasePlayerController::HIDE_UI_SETTING_KEY,
		ABasePlayerController::DEFAULT_HIDE_UI));
}



// ------------------------------------------------------------------------- Crosshair --------------------------------------------------------------------------

float UGeneralSettingsUI::NormalizeCrosshairLinesGap(float Gap)
{
	if (Gap < 0)
	{
		Gap = 0;
	}

	return Gap;
}

void UGeneralSettingsUI::SetCrosshairLinesGap(float Gap)
{
	Gap = NormalizeCrosshairLinesGap(Gap);
	OnCrosshairLinesGapUpdated(Gap);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::CROSSHAIR_LINES_GAP_SETTING_KEY, Gap);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

float UGeneralSettingsUI::NormalizeCrosshairLinesLength(float Length)
{
	if (Length < 0)
	{
		Length = 0;
	}

	return Length;
}

void UGeneralSettingsUI::SetCrosshairLinesLength(float Length)
{
	Length = NormalizeCrosshairLinesLength(Length);
	OnCrosshairLinesLengthUpdated(Length);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::CROSSHAIR_LINES_LENGTH_SETTING_KEY, Length);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

float UGeneralSettingsUI::NormalizeCrosshairLinesThickness(float Thickness)
{
	if (Thickness < 0)
	{
		Thickness = 0;
	}

	return Thickness;
}

void UGeneralSettingsUI::SetCrosshairLinesThickness(float Thickness)
{
	Thickness = NormalizeCrosshairLinesThickness(Thickness);
	OnCrosshairLinesThicknessUpdated(Thickness);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::CROSSHAIR_LINES_THICKNESS_SETTING_KEY, Thickness);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

float UGeneralSettingsUI::NormalizeCrosshairInaccuracyScale(float Scale)
{
	if (Scale < 0)
	{
		Scale = 0;
	}

	return Scale;
}

void UGeneralSettingsUI::SetCrosshairInaccuracyScale(float Scale)
{
	Scale = NormalizeCrosshairInaccuracyScale(Scale);
	OnCrosshairInaccuracyScaleUpdated(Scale);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::CROSSHAIR_INACCURACY_SCALE_SETTING_KEY, Scale);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}



// ------------------------------------------------------------------------- Map and Minimap --------------------------------------------------------------------------

int UGeneralSettingsUI::NormalizeMinimapSize(int Size)
{
	if (Size < 0)
	{
		Size = 0;
	}

	return Size;
}

void UGeneralSettingsUI::SetMinimapSize(int Size)
{
	Size = NormalizeMinimapSize(Size);
	OnMinimapSizeUpdated(Size);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::General, ABasePlayerController::MINIMAP_SIZE_SETTING_KEY, Size);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

float UGeneralSettingsUI::NormalizeMinimapZoom(float Zoom)
{
	if (Zoom < 0)
	{
		Zoom = 0;
	}

	return Zoom;
}

void UGeneralSettingsUI::SetMinimapZoom(float Zoom)
{
	Zoom = NormalizeMinimapZoom(Zoom);
	OnMinimapZoomUpdated(Zoom);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::MINIMAP_ZOOM_SETTING_KEY, Zoom);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

float UGeneralSettingsUI::NormalizeMinimapIconsScale(float Scale)
{
	if (Scale < 0)
	{
		Scale = 0;
	}

	return Scale;
}

void UGeneralSettingsUI::SetMinimapIconsScale(float Scale)
{
	Scale = NormalizeMinimapIconsScale(Scale);
	OnMinimapIconsScaleUpdated(Scale);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::MINIMAP_ICONS_SCALE_SETTING_KEY, Scale);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

float UGeneralSettingsUI::NormalizeMapIconsScale(float Scale)
{
	if (Scale < 0)
	{
		Scale = 0;
	}

	return Scale;
}

void UGeneralSettingsUI::SetMapIconsScale(float Scale)
{
	Scale = NormalizeMapIconsScale(Scale);
	OnMapIconsScaleUpdated(Scale);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::General, ABasePlayerController::MAP_ICONS_SCALE_SETTING_KEY, Scale);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}

void UGeneralSettingsUI::SetHideUI(bool HideUI)
{
	OnHideUIUpdated(HideUI);

	GetGameInstance<UStalkerMPGameInstance>()->PutIntSettingValue(ESettingsType::General, ABasePlayerController::HIDE_UI_SETTING_KEY, HideUI ? 1 : 0);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::General);
}
