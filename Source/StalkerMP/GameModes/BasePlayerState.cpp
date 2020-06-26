// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerState.h"

#include "StalkerMP/GameModes/BaseGameState.h"

#include "Net/UnrealNetwork.h"

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePlayerState, KillCount);
	DOREPLIFETIME(ABasePlayerState, DeathCount);
}

void ABasePlayerState::SetGameState(ABaseGameState* TheGameState)
{
	GameState = TheGameState;
	GameState->OnPlayerStateUpdated(this);
}

void ABasePlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	if (GameState)
	{
		GameState->OnPlayerStateUpdated(this);
	}
}

void ABasePlayerState::OnRep_KillCount()
{
	if (GameState)
	{
		GameState->OnPlayerStateUpdated(this);
	}
}

void ABasePlayerState::OnRep_DeathCount()
{
	if (GameState)
	{
		GameState->OnPlayerStateUpdated(this);
	}
}
