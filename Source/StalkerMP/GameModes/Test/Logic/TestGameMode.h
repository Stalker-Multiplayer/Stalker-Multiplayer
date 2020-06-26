// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BaseGameMode.h"

#include "TestGameMode.generated.h"

class ATestPlayerController;

class APawn;

/**
 * 
 */
UCLASS()
class STALKERMP_API ATestGameMode : public ABaseGameMode
{
	GENERATED_BODY()
	

// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void OnGamemodeLevelLoaded() override;
	virtual FString GetGameModeName() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* PlayerController) override;
	virtual void Logout(AController* Exiting) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:
		
	UPROPERTY()
		TArray<AActor*> AllPlayerStarts;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void RespawnAsPawn(ATestPlayerController* PlayerController, TSubclassOf<APawn> PawnClass, FTransform SpawnTransform);


};
