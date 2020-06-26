// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

class ALobbyPlayerState;

/**
 * 
 */
UCLASS()
class STALKERMP_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function overrides --------------------------------------------------------------------------

public:
	// Sets default values for this pawn's properties
	ALobbyGameMode();




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString LevelToLoad;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	bool AllPlayersReady();


public:

	void OnPlayerReady(ALobbyPlayerState* LobbyPlayerState);


};
