// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "GameFramework/GameStateBase.h"
#include "BaseGameState.generated.h"

class ABasePlayerController;
class ABasePlayerState;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	
// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	virtual void AddPlayerState(APlayerState* PlayerState) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
		EMatchState MatchState = EMatchState::InLobby;


protected:

	UPROPERTY()
		TArray<ABasePlayerController*> LocalPlayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_JoinedPlayersCount)
		int JoinedPlayersCount = 0;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------
protected:

	UFUNCTION()
		virtual void OnRep_MatchState();

	UFUNCTION()
		virtual void OnRep_JoinedPlayersCount();


public:

	UFUNCTION()
		virtual void AddLocalPlayer(ABasePlayerController* LocalPlayer) { LocalPlayers.AddUnique(LocalPlayer); }

	UFUNCTION()
		virtual void RemoveLocalPlayer(ABasePlayerController* LocalPlayer) { LocalPlayers.Remove(LocalPlayer); }

	UFUNCTION()
		virtual void OnPlayerStateUpdated(ABasePlayerState* PlayerState);

	UFUNCTION()
		void SetMatchState(EMatchState NewMatchState) { MatchState = NewMatchState; }

	UFUNCTION()
		EMatchState GetMatchState() { return MatchState; }

	UFUNCTION()
		void SetJoinedPlayersCount(int NewCount) { JoinedPlayersCount = NewCount; }

	
};
