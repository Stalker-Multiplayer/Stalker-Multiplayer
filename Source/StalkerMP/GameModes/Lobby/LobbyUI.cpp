// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyUI.h"

#include "StalkerMP/GameModes/Lobby/LobbyPlayerState.h"
#include "StalkerMP/GameModes/Lobby/LobbyPlayerController.h"



void ULobbyUI::ToggleReady()
{
	GetOwningPlayer<ALobbyPlayerController>()->ToggleReady();
}
