// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BaseGameMode.h"

#include "Misc/Timecode.h"
#include "BRGameMode.generated.h"

class ABRZoneActor;
class ABRBlowoutActor;
class AController;
class APlayerStart;
class APlayerStartPIE;
class APlayerCharacter;
class ASpectator;
class AItemSpawner;
class ABRGameState;

USTRUCT(BlueprintType)
struct FBRGrantableItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class ABaseItem> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool IfWeaponThenFullyLoaded = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<class ABaseItem>> SatelliteItems;

};

USTRUCT(BlueprintType)
struct FBRItemsToGrantOnSpawnList
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FBRGrantableItem> ItemsList;

};

/**
 * 
 */
UCLASS()
class STALKERMP_API ABRGameMode : public ABaseGameMode
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const int NEXT_START_TIME_MIN_DELTA = 7200; // 2 hours


public:
	
	static const FString MIN_PLAYERS_TO_START_SETTING_KEY;
	static const FString GAME_START_DELAY_SETTING_KEY;
	static const FString POST_FINISH_RESET_DELAY_SETTING_KEY;
	static const FString EARLIEST_START_TIME_SETTING_KEY;
	static const FString LATEST_START_TIME_SETTING_KEY;
	static const FString GAME_TIME_PASS_SETTING_KEY;
	static const FString USE_ZONE_CIRCLE;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* PlayerController) override;
	virtual void Logout(AController* Exiting) override;

	virtual void OnGamemodeLevelLoaded() override;
	virtual FString GetGameModeName() override;
	virtual void StartGame() override;
	virtual void StopGame() override;
	virtual void RestartGame() override;
	virtual void OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser, ADeadBodyBackpack* DeadBodyBackpack) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		TArray<AItemSpawner*> ItemSpawners;

	UPROPERTY()
		bool UseZoneCircle = false;

	UPROPERTY()
		ABRZoneActor* ZoneActor;

	UPROPERTY()
		ABRBlowoutActor* BlowoutActor;

	UPROPERTY()
		FTimerHandle StartCountdownTimerHandle;

	UPROPERTY()
		FTimerHandle AfterGameTimerHandle;

	UPROPERTY()
		TArray<APlayerCharacter*> AliveCharacters;

	UPROPERTY()
		FTimecode MatchStartTime;


protected:

	UPROPERTY(BlueprintReadOnly)
		int MinPlayersToStart = 2;

	UPROPERTY(BlueprintReadOnly)
		int StartGameDelay = 30;

	UPROPERTY(BlueprintReadOnly)
		int GameStartTimer = StartGameDelay;

	UPROPERTY(BlueprintReadOnly)
		int RestartGameDelay = 10;

	UPROPERTY(BlueprintReadOnly)
		FTimecode EarliestStartTime;

	UPROPERTY(BlueprintReadOnly)
		FTimecode LatestStartTime;

	UPROPERTY(BlueprintReadOnly)
		FTimecode GameTimePass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FBRItemsToGrantOnSpawnList> ItemsToGrantOnSpawn;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void GrantStartingItems(APlayerCharacter* PlayerCharacter);

	UFUNCTION()
		void UpdateGameStartTimer();

	UFUNCTION()
		void LaunchGameStartTimer();

};
