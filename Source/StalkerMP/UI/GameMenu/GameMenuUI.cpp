// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenuUI.h"

#include "StalkerMP/GameModes/BasePlayerController.h"


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

FString UGameMenuUI::GetGameVersion()
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

void UGameMenuUI::Resume()
{
	PlayerController->ShowGameMenuUI(false);
}

void UGameMenuUI::OpenSettings()
{
	PlayerController->ShowSettingsUI(true);
}

void UGameMenuUI::ExitToMainMenu()
{
	PlayerController->ExitToMainMenu();
}

void UGameMenuUI::QuitToDesktop()
{
	PlayerController->QuitToDesktop();
}
