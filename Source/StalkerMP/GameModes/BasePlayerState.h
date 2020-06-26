// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void OnRep_PlayerName() override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		ABaseGameState* GameState;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_KillCount)
		int KillCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_DeathCount)
		int DeathCount = 0;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		virtual void OnRep_KillCount();

	UFUNCTION()
		virtual void OnRep_DeathCount();


public:

	UFUNCTION()
		void SetGameState(ABaseGameState* TheGameState);


};
