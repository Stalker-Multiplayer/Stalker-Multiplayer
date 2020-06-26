// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerState.h"

#include "StalkerMP/GameModes/Lobby/LobbyGameMode.h"
#include "StalkerMP/GameModes/Lobby/LobbyGameState.h"

#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"


void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, IsReady);
}

void ALobbyPlayerState::ToggleReady()
{
	IsReady = !IsReady;

	Server_ToggleReady();
}

void ALobbyPlayerState::Server_ToggleReady_Implementation()
{
	IsReady = !IsReady;

	if (IsReady)
	{
		GetWorld()->GetAuthGameMode<ALobbyGameMode>()->OnPlayerReady(this);
	}
}

bool ALobbyPlayerState::Server_ToggleReady_Validate()
{
	return true;
}
