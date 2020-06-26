// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuPlayerController.h"

#include "StalkerMP/GameModes/MainMenu/MainMenuUI.h"
#include "StalkerMP/GameModes/MainMenu/ServerBrowserUI.h"
#include "StalkerMP/UI/GameMenu/SettingsUI.h"
#include "StalkerMP/UI/LoadingUI.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		ConsoleCommand("t.MaxFPS 60");

		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;

		MainMenuUI = CreateWidget<UMainMenuUI>(this, MainMenuUIClass);
		MainMenuUI->AddToViewport(MAIN_MENU_ZORDER);

		ServerBrowserUI = CreateWidget<UServerBrowserUI>(this, ServerBrowserUIClass);
		ServerBrowserUI->AddToViewport(SERVER_BROWSER_ZORDER);

		LoadingUI = CreateWidget<ULoadingUI>(this, LoadingUIClass);
		LoadingUI->AddToViewport(LOADING_UI_ZORDER);

		ShowMainMenu();

		UGameplayStatics::PlaySound2D(GetWorld(), BackgroundAudio);
	}
}

void AMainMenuPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (MainMenuUI)
	{
		MainMenuUI->RemoveFromParent();
		MainMenuUI = nullptr;
	}
	if (ServerBrowserUI)
	{
		ServerBrowserUI->RemoveFromParent();
		ServerBrowserUI = nullptr;
	}
	if (LoadingUI)
	{
		LoadingUI->RemoveFromParent();
		LoadingUI = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

bool AMainMenuPlayerController::IsAnyMenuVisible()
{
	return true;
}

void AMainMenuPlayerController::OnNetworkFailure(ENetworkFailure::Type FailureType, FString ErrorString)
{
	ShowServerBrowser();
	ServerBrowserUI->ShowErrorMessage(true, FText::FromString(ErrorString));
}

void AMainMenuPlayerController::OnMenuButtonPressed()
{
	if (SettingsUI->GetVisibility() == ESlateVisibility::Visible)
	{
		ShowSettingsUI(false);

		return;
	}
}

void AMainMenuPlayerController::ConnectToServer(FName ServerAddress)
{
	UGameplayStatics::OpenLevel(GetWorld(), ServerAddress);
}

void AMainMenuPlayerController::ShowMainMenu()
{
	MainMenuUI->SetVisibility(ESlateVisibility::Visible);
	ServerBrowserUI->SetVisibility(ESlateVisibility::Collapsed);
	LoadingUI->SetVisibility(ESlateVisibility::Collapsed);
}

void AMainMenuPlayerController::ShowServerBrowser()
{
	MainMenuUI->SetVisibility(ESlateVisibility::Collapsed);
	ServerBrowserUI->SetVisibility(ESlateVisibility::Visible);
	ServerBrowserUI->RefreshList();
	LoadingUI->SetVisibility(ESlateVisibility::Collapsed);
}

void AMainMenuPlayerController::ShowLoadingScreen()
{
	MainMenuUI->SetVisibility(ESlateVisibility::Collapsed);
	ServerBrowserUI->SetVisibility(ESlateVisibility::Collapsed);
	LoadingUI->SetVisibility(ESlateVisibility::Visible);
	ShowSettingsUI(false);
}