// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY(Replicated)
		bool IsReady = false;
	



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleReady();


public:

	UFUNCTION()
		void ToggleReady();

	UFUNCTION()
		bool GetIsReady() { return IsReady; }


};
