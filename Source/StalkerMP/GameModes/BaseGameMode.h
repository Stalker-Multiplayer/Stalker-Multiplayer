// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "OnlineDelegateMacros.h"
#include "BaseGameMode.generated.h"

class ABasePlayerController;
class APlayerCharacter;
class ADeadBodyBackpack;
class ASpectator;
class AWeatherActor;

class APlayerStartPIE;
class AController;
class AActor;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseGameMode();



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* PlayerController) override;
	virtual void Logout(AController* Exiting) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnPlayerCharacterDiedDelegate;
	FScriptDelegate OnGamemodeLevelLoadedDelegate;


protected:

	UPROPERTY()
		AWeatherActor* WeatherActor;

	UPROPERTY()
		APlayerStartPIE* PlayerStartPIE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ASpectator> PlayerSpectatorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<APlayerCharacter> PlayerCharacterClass;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION()
		virtual void OnGamemodeLevelLoaded();

	UFUNCTION()
		virtual FString GetGameModeName();

	UFUNCTION()
		virtual void StartGame();

	UFUNCTION()
		virtual void StopGame();

	UFUNCTION()
		virtual void RestartGame();


public:

	UFUNCTION()
		APawn* RespawnAsPawn(ABasePlayerController* PlayerController, TSubclassOf<APawn> PawnClass, FTransform SpawnTransform, bool AutoPossess = true, bool DestroyOldPawn = false);

	UFUNCTION()
		virtual void OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser, ADeadBodyBackpack* DeadBodyBackpack);


};
