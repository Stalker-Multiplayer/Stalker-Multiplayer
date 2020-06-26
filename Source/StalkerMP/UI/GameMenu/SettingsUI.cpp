// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingsUI.h"

#include "StalkerMP/UI/GameMenu/GeneralSettingsUI.h"
#include "StalkerMP/UI/GameMenu/VideoSettingsUI.h"
#include "StalkerMP/UI/GameMenu/SoundSettingsUI.h"
#include "StalkerMP/UI/GameMenu/ControlsSettingsUI.h"
#include "StalkerMP/GameModes/BasePlayerController.h"


void USettingsUI::NativeConstruct()
{
	Super::NativeConstruct();

	ShowGeneralSettingsUI();
}

void USettingsUI::SetPlayerController(ABasePlayerController* ThePlayerController)
{
	Super::SetPlayerController(ThePlayerController);

	GetGeneralSettingsUI()->SetPlayerController(ThePlayerController);
	GetVideoSettingsUI()->SetPlayerController(ThePlayerController);
	GetSoundSettingsUI()->SetPlayerController(ThePlayerController);
	GetControlsSettingsUI()->SetPlayerController(ThePlayerController);
}

void USettingsUI::HideSubSettings()
{
	GetGeneralSettingsUI()->SetVisibility(ESlateVisibility::Collapsed);
	GetVideoSettingsUI()->SetVisibility(ESlateVisibility::Collapsed);
	GetSoundSettingsUI()->SetVisibility(ESlateVisibility::Collapsed);
	GetControlsSettingsUI()->SetVisibility(ESlateVisibility::Collapsed);
}

void USettingsUI::ShowGeneralSettingsUI()
{
	HideSubSettings();
	GetGeneralSettingsUI()->SetVisibility(ESlateVisibility::Visible);
}

void USettingsUI::ShowVideoSettingsUI()
{
	HideSubSettings();
	GetVideoSettingsUI()->SetVisibility(ESlateVisibility::Visible);
}

void USettingsUI::ShowSoundSettingsUI()
{
	HideSubSettings();
	GetSoundSettingsUI()->SetVisibility(ESlateVisibility::Visible);
}

void USettingsUI::ShowControlsSettingsUI()
{
	HideSubSettings();
	GetControlsSettingsUI()->SetVisibility(ESlateVisibility::Visible);
}

void USettingsUI::CloseSettings()
{
	PlayerController->ShowSettingsUI(false);
}