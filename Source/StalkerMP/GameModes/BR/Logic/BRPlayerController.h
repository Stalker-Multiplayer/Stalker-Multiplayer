// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BasePlayerController.h"

#include "BRPlayerController.generated.h"

class UBRGameStartingUI;
class UBRGameUI;
class UBRGameEndedUI;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABRPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void PawnLeavingGame() override;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------


public:

	UFUNCTION()
		void UpdateGameStartTimer(int NewTimer);

	UFUNCTION()
		void UpdateJoinedPlayersCount(int NewCount, int MinPlayersToStart);

	UFUNCTION()
		void UpdatePlayersAliveCount(int NewCount);

	
};
