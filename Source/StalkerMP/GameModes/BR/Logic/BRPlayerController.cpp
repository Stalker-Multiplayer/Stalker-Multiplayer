// Fill out your copyright notice in the Description page of Project Settings.

#include "BRPlayerController.h"

#include "StalkerMP/GameModes/BR/UI/BRGameStartingUI.h"
#include "StalkerMP/GameModes/BR/UI/BRGameUI.h"
#include "StalkerMP/GameModes/BR/UI/BRGameEndedUI.h"
#include "StalkerMP/StalkerMPGameInstance.h"


void ABRPlayerController::PawnLeavingGame()
{
	// do nothing for now
}

void ABRPlayerController::UpdateGameStartTimer(int NewTimer)
{
	if (CustomGameStartingUI)
	{
		((UBRGameStartingUI*) CustomGameStartingUI)->UpdateGameStartTimer(NewTimer);
	}
}

void ABRPlayerController::UpdateJoinedPlayersCount(int NewCount, int MinPlayersToStart)
{
	if (CustomGameStartingUI)
	{
		((UBRGameStartingUI*) CustomGameStartingUI)->UpdateJoinedPlayersCount(NewCount, MinPlayersToStart);
	}
}

void ABRPlayerController::UpdatePlayersAliveCount(int NewCount)
{
	if (CustomGameUI)
	{
		((UBRGameUI*) CustomGameUI)->UpdatePlayersAliveCount(NewCount);
	}
}
