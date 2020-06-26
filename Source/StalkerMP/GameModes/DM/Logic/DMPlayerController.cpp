// Fill out your copyright notice in the Description page of Project Settings.

#include "DMPlayerController.h"

#include "StalkerMP/GameModes/DM/UI/DMGameStartingUI.h"
#include "StalkerMP/GameModes/DM/UI/DMGameUI.h"
#include "StalkerMP/GameModes/DM/UI/DMGameEndedUI.h"
#include "StalkerMP/GameModes/DM/Logic/DMPlayerState.h"
#include "StalkerMP/GameModes/DM/Logic/DMGameState.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Engine.h"


void ADMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (GetWorld()->GetGameState<ADMGameState>())
	{
		GetWorld()->GetGameState<ADMGameState>()->OnPlayerStateUpdated((ADMPlayerState*)PlayerState);
	}
}

void ADMPlayerController::UpdateGameStartTimer(int NewTimer)
{
	if (CustomGameStartingUI)
	{
		((UDMGameStartingUI*) CustomGameStartingUI)->UpdateGameStartTimer(NewTimer);
	}
}

void ADMPlayerController::UpdateJoinedPlayersCount(int NewCount, int MinPlayersToStart)
{
	if (CustomGameStartingUI)
	{
		((UDMGameStartingUI*) CustomGameStartingUI)->UpdateJoinedPlayersCount(NewCount, MinPlayersToStart);
	}
}

void ADMPlayerController::UpdateTimeLeft(int NewTimeLeft)
{
	if (CustomGameUI)
	{
		((UDMGameUI*)CustomGameUI)->UpdateTimeLeft(NewTimeLeft);
	}
}

void ADMPlayerController::UpdateScoreLimit(int NewScoreLimit)
{
	if (CustomGameUI)
	{
		((UDMGameUI*)CustomGameUI)->UpdateScoreLimit(NewScoreLimit);
	}
}

void ADMPlayerController::UpdatePlayersRanks(TArray<ADMPlayerState*> Players)
{
	if (CustomGameUI)
	{
		((UDMGameUI*)CustomGameUI)->UpdatePlayersRanks(Players, Players.Find((ADMPlayerState*) PlayerState));
	}
}
