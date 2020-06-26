// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerListEntryWidget.h"

#include "StalkerMP/GameModes/Lobby/LobbyPlayerState.h"



void ULobbyPlayerListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ALobbyPlayerState* TheLobbyPlayerState = dynamic_cast<ALobbyPlayerState*>(ListItemObject)) {
		LobbyPlayerState = TheLobbyPlayerState;
	}
}

FString ULobbyPlayerListEntryWidget::GetPlayerName()
{
	return LobbyPlayerState->GetPlayerName();
}

bool ULobbyPlayerListEntryWidget::GetIsPlayerReady()
{
	return LobbyPlayerState->GetIsReady();
}
