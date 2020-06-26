// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BaseGameState.h"

#include "BRGameState.generated.h"

class ABRPlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABRGameState : public ABaseGameState
{
	GENERATED_BODY()
	
	


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	virtual void OnRep_JoinedPlayersCount() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_GameStartTimer)
		int GameStartTimer = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MinPlayersToStart)
		int MinPlayersToStart = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_PlayersAliveCount)
		int PlayersAliveCount = 0;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnRep_GameStartTimer();

	UFUNCTION()
		void OnRep_MinPlayersToStart();

	UFUNCTION()
		void OnRep_PlayersAliveCount();

	
};
