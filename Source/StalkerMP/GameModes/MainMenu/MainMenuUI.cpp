// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuUI.h"

#include "StalkerMP/GameModes/MainMenu/MainMenuPlayerController.h"


FString UMainMenuUI::GetGameVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}

void UMainMenuUI::OpenServerBrowser()
{
	GetOwningPlayer<AMainMenuPlayerController>()->ShowServerBrowser();
}

void UMainMenuUI::OpenSettings()
{
	GetOwningPlayer<AMainMenuPlayerController>()->ShowSettingsUI(true);
}

void UMainMenuUI::QuitGame()
{
	GetOwningPlayer<AMainMenuPlayerController>()->QuitToDesktop();
}
