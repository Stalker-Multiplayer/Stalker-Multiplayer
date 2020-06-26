// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BasePlayerController.h"

#include "DMPlayerController.generated.h"

class UDMGameStartingUI;
class UDMGameUI;
class UDMGameEndedUI;
class ADMPlayerState;

/**
 * 
 */
UCLASS()
class STALKERMP_API ADMPlayerController : public ABasePlayerController
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void OnRep_PlayerState() override;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void UpdateGameStartTimer(int NewTimer);

	UFUNCTION()
		void UpdateJoinedPlayersCount(int NewCount, int MinPlayersToStart);

	UFUNCTION()
		void UpdateTimeLeft(int NewTimeLeft);

	UFUNCTION()
		void UpdateScoreLimit(int NewScoreLimit);

	UFUNCTION()
		void UpdatePlayersRanks(TArray<ADMPlayerState*> Players);

};
