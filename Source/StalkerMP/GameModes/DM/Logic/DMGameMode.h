// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BaseGameMode.h"

#include "DMGameMode.generated.h"

class ADMPlayerController;

USTRUCT(BlueprintType)
struct FDMGrantableItem
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
struct FDMItemsToGrantOnSpawnList
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FDMGrantableItem> ItemsList;

};

/**
 * 
 */
UCLASS()
class STALKERMP_API ADMGameMode : public ABaseGameMode
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

public:

	static const FString MIN_PLAYERS_TO_START_SETTING_KEY;
	static const FString GAME_START_DELAY_SETTING_KEY;
	static const FString POST_FINISH_RESET_DELAY_SETTING_KEY;
	static const FString TIME_LIMIT;
	static const FString SCORE_LIMIT;
	static const FString DEAD_BODY_REMOVE_DELAY;
	static const FString DROPPED_ITEMS_REMOVE_DELAY;
	static const FString EARLIEST_START_TIME_SETTING_KEY;
	static const FString LATEST_START_TIME_SETTING_KEY;
	static const FString GAME_TIME_PASS_SETTING_KEY;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnGamemodeLevelLoaded() override;
	virtual FString GetGameModeName() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* PlayerController) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartGame() override;
	virtual void RestartGame() override;
	virtual void OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser, ADeadBodyBackpack* DeadBodyBackpack) override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnItemPickedUpDelegate;
	FScriptDelegate OnItemDroppedDelegate;


	UPROPERTY()
		FTimerHandle StartCountdownTimerHandle;

	UPROPERTY()
		FTimerHandle AfterGameTimerHandle;

	UPROPERTY()
		FTimerHandle GameTimerHandle;

	UPROPERTY()
		TMap<AController*, FTimerHandle> Map_RespawnPlayerTimerHandles;
	
	UPROPERTY()
		TArray<AActor*> AllPlayerStarts;


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

	UPROPERTY(BlueprintReadOnly)
		int TimeLimit = 600;

	UPROPERTY(BlueprintReadOnly)
		int TimeLeft = TimeLimit;

	UPROPERTY(BlueprintReadOnly)
		int ScoreLimit = 30;

	UPROPERTY(BlueprintReadOnly)
		int DeadBodyRemoveDelay = 5;

	UPROPERTY(BlueprintReadOnly)
		int DroppedItemsRemoveDelay = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FDMItemsToGrantOnSpawnList> ItemsToGrantOnSpawn;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void GrantStartingItems(APlayerCharacter* PlayerCharacter);

	UFUNCTION()
		void UpdateGameStartTimer();

	UFUNCTION()
		void LaunchGameStartTimer();

	UFUNCTION()
		void UpdateGameTimer();

	UFUNCTION()
		void RespawnPlayer(ADMPlayerController* PlayerController);

	UFUNCTION()
		void OnItemPickedUp(ABaseItem* PickedItem);

	UFUNCTION()
		void OnItemDropped(ABaseItem* DroppedItem);


};
