// Fill out your copyright notice in the Description page of Project Settings.

#include "BRGameState.h"

#include "StalkerMP/GameModes/BR/Logic/BRPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"



void ABRGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABRGameState, GameStartTimer);
	DOREPLIFETIME(ABRGameState, MinPlayersToStart);
	DOREPLIFETIME(ABRGameState, PlayersAliveCount);
}

void ABRGameState::OnRep_JoinedPlayersCount()
{
	Super::OnRep_JoinedPlayersCount();

	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ABRPlayerController*)LocalPlayer)->UpdateJoinedPlayersCount(JoinedPlayersCount, MinPlayersToStart);
	}
}

void ABRGameState::OnRep_GameStartTimer()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ABRPlayerController*) LocalPlayer)->UpdateGameStartTimer(GameStartTimer);
	}
}

void ABRGameState::OnRep_MinPlayersToStart()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ABRPlayerController*)LocalPlayer)->UpdateJoinedPlayersCount(JoinedPlayersCount, MinPlayersToStart);
	}
}

void ABRGameState::OnRep_PlayersAliveCount()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ABRPlayerController*)LocalPlayer)->UpdatePlayersAliveCount(PlayersAliveCount);
	}
}
