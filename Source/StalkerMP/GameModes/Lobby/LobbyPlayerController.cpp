// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"

#include "StalkerMP/GameModes/Lobby/LobbyUI.h"
#include "StalkerMP/GameModes/Lobby/LobbyGameState.h"
#include "StalkerMP/GameModes/Lobby/LobbyPlayerState.h"
#include "StalkerMP/UI/LoadingUI.h"

#include "Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AudioDevice.h"


// Called when the game starts or when spawned
void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		ConsoleCommand("t.MaxFPS 60");

		LobbyUI = CreateWidget<ULobbyUI>(this, LobbyUIClass);
		LobbyUI->AddToViewport(LOBBY_UI_ZORDER);

		GetWorld()->GetGameState<ALobbyGameState>()->SetLobbyUI(LobbyUI);

		UGameplayStatics::PlaySound2D(GetWorld(), BackgroundAudio, 1);
		PlayRandomGuitarTrack();
	}
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (LobbyUI)
	{
		LobbyUI->RemoveFromParent();
		LobbyUI = nullptr;
	}
	if (LoadingUI)
	{
		LoadingUI->RemoveFromParent();
		LobbyUI = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void ALobbyPlayerController::WaitToPlayGuitar()
{
	int WaitTime = MinMaxGuitarDelay.X + UKismetMathLibrary::RandomInteger(MinMaxGuitarDelay.Y - MinMaxGuitarDelay.X);
	GetWorld()->GetTimerManager().SetTimer(GuitarTimerHandle, this, &ALobbyPlayerController::PlayRandomGuitarTrack, WaitTime, false);
}

void ALobbyPlayerController::PlayRandomGuitarTrack()
{
	UAudioComponent* GuitarTrack = UGameplayStatics::SpawnSound2D(GetWorld(), GuitarTracks[UKismetMathLibrary::RandomInteger(GuitarTracks.Num())], 1);
	GuitarTrack->OnAudioFinished.AddDynamic(this, &ALobbyPlayerController::WaitToPlayGuitar);
}

void ALobbyPlayerController::Server_ToggleReady_Implementation()
{
	GetPlayerState<ALobbyPlayerState>()->ToggleReady();
}

bool ALobbyPlayerController::Server_ToggleReady_Validate()
{
	return true;
}

void ALobbyPlayerController::Client_NotifyLoading_Implementation()
{
	if (LobbyUI)
	{
		LobbyUI->RemoveFromParent();
		LobbyUI = nullptr;
	}

	LoadingUI = CreateWidget<ULoadingUI>(this, LoadingUIClass);
	LoadingUI->AddToViewport(LOADING_UI_ZORDER);
}

void ALobbyPlayerController::ToggleReady()
{
	Server_ToggleReady();

	GetPlayerState<ALobbyPlayerState>()->ToggleReady();
}
