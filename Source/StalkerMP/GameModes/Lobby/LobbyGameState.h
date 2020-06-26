// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

class ULobbyUI;
class ALobbyPlayerState;

/**
 * 
 */
UCLASS()
class STALKERMP_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	/** Add PlayerState to the PlayerArray */
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	/** Remove PlayerState from the PlayerArray. */
	virtual void RemovePlayerState(APlayerState* PlayerState) override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		ULobbyUI* LobbyUI;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		bool AllPlayersReady();

	UFUNCTION()
		void SetLobbyUI(ULobbyUI* TheLobbyUI);


	
};
