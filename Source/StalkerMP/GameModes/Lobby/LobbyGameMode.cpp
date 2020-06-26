// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "StalkerMP/GameModes/Lobby/LobbyGameState.h"
#include "StalkerMP/GameModes/Lobby/LobbyPlayerState.h"
#include "StalkerMP/GameModes/Lobby/LobbyPlayerController.h"

#include "EngineUtils.h"


ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
	bPauseable = false;
}

bool ALobbyGameMode::AllPlayersReady()
{
	return GetGameState<ALobbyGameState>()->AllPlayersReady();
}

void ALobbyGameMode::OnPlayerReady(ALobbyPlayerState* LobbyPlayerState)
{
	if (AllPlayersReady())
	{
		for (TActorIterator<ALobbyPlayerController> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			ALobbyPlayerController* LobbyPlayerController = *ActorItr;

			LobbyPlayerController->Client_NotifyLoading();
		}

		GetWorld()->ServerTravel(LevelToLoad);
	}
}


