// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameState.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/GameModes/BasePlayerState.h"

#include "Net/UnrealNetwork.h"


void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseGameState, MatchState);
	DOREPLIFETIME(ABaseGameState, JoinedPlayersCount);
}

void ABaseGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	((ABasePlayerState*)PlayerState)->SetGameState(this);
}

void ABaseGameState::OnPlayerStateUpdated(ABasePlayerState* PlayerState)
{
	// Do nothing
}

void ABaseGameState::OnRep_MatchState()
{
	for (ABasePlayerController* LocalPlayer : LocalPlayers)
	{
		LocalPlayer->OnMatchStateUpdated(MatchState);
	}
}

void ABaseGameState::OnRep_JoinedPlayersCount()
{

}
