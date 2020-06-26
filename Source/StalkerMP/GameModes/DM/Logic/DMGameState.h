// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BaseGameState.h"

#include "DMGameState.generated.h"

class ADMPlayerState;
class ADMPlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API ADMGameState : public ABaseGameState
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void OnRep_JoinedPlayersCount() override;
	virtual void AddLocalPlayer(ABasePlayerController* LocalPlayer) override;

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void OnPlayerStateUpdated(ABasePlayerState* PlayerState) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		TArray<ADMPlayerState*> SortedPlayers;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_GameStartTimer)
		int GameStartTimer = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MinPlayersToStart)
		int MinPlayersToStart = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TimeLeft)
		int TimeLeft = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_ScoreLimit)
		int ScoreLimit = -1;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnRep_GameStartTimer();

	UFUNCTION()
		void OnRep_MinPlayersToStart();

	UFUNCTION()
		void OnRep_TimeLeft();

	UFUNCTION()
		void OnRep_ScoreLimit();

	UFUNCTION()
		void SortPlayersByKills();


};
