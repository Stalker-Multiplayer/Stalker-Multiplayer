// Fill out your copyright notice in the Description page of Project Settings.

#include "DMGameState.h"

#include "StalkerMP/GameModes/DM/Logic/DMPlayerController.h"
#include "StalkerMP/GameModes/DM/Logic/DMPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

void ADMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADMGameState, GameStartTimer);
	DOREPLIFETIME(ADMGameState, MinPlayersToStart);
	DOREPLIFETIME(ADMGameState, TimeLeft);
	DOREPLIFETIME(ADMGameState, ScoreLimit);
}

void ADMGameState::OnRep_JoinedPlayersCount()
{
	Super::OnRep_JoinedPlayersCount();

	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ADMPlayerController*)LocalPlayer)->UpdateJoinedPlayersCount(JoinedPlayersCount, MinPlayersToStart);
	}
}

void ADMGameState::OnRep_GameStartTimer()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ADMPlayerController*)LocalPlayer)->UpdateGameStartTimer(GameStartTimer);
	}
}

void ADMGameState::OnRep_MinPlayersToStart()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ADMPlayerController*)LocalPlayer)->UpdateJoinedPlayersCount(JoinedPlayersCount, MinPlayersToStart);
	}
}

void ADMGameState::OnRep_TimeLeft()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ADMPlayerController*)LocalPlayer)->UpdateTimeLeft(TimeLeft);
	}
}

void ADMGameState::OnRep_ScoreLimit()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ADMPlayerController*)LocalPlayer)->UpdateScoreLimit(ScoreLimit);
	}
}

void ADMGameState::AddLocalPlayer(ABasePlayerController* LocalPlayer)
{
	Super::AddLocalPlayer(LocalPlayer);

	SortPlayersByKills();
}

void ADMGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (!HasAuthority())
	{
		ADMPlayerState* DMPlayerState = (ADMPlayerState*)PlayerState;
		if (!SortedPlayers.Contains(DMPlayerState))
		{
			SortedPlayers.AddUnique(DMPlayerState);
		}
		SortPlayersByKills();
	}
}

void ADMGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	if (!HasAuthority())
	{
		SortedPlayers.Remove((ADMPlayerState*)PlayerState);
		SortPlayersByKills();
	}
}

void ADMGameState::OnPlayerStateUpdated(ABasePlayerState* PlayerState)
{
	SortPlayersByKills();
}

void ADMGameState::SortPlayersByKills()
{
	SortedPlayers.Sort([](const ADMPlayerState& l, const ADMPlayerState& r) { return l.KillCount > r.KillCount; });

	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		((ADMPlayerController*)LocalPlayer)->UpdatePlayersRanks(SortedPlayers);
	}
}
