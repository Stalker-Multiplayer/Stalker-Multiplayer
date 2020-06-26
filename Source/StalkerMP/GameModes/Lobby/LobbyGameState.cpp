// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameState.h"

#include "StalkerMP/GameModes/Lobby/LobbyUI.h"
#include "StalkerMP/GameModes/Lobby/LobbyPlayerState.h"




void ALobbyGameState::SetLobbyUI(ULobbyUI* TheLobbyUI)
{
	LobbyUI = TheLobbyUI;
	LobbyUI->SetPlayersList(PlayerArray);
}

void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (LobbyUI) {
		LobbyUI->SetPlayersList(PlayerArray);
	}
}

void ALobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	if (LobbyUI) {
		LobbyUI->SetPlayersList(PlayerArray);
	}
}

bool ALobbyGameState::AllPlayersReady()
{
	if (PlayerArray.Num() < 2)
	{
		return false;
	}

	for (APlayerState* PlayerState : PlayerArray)
	{
		if (ALobbyPlayerState* LobbyPlayerState = dynamic_cast<ALobbyPlayerState*>(PlayerState)) {
			if (!LobbyPlayerState->GetIsReady())
			{
				return false;
			}
		}
	}

	return true;
}
