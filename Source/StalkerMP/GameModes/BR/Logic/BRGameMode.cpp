// Fill out your copyright notice in the Description page of Project Settings.

#include "BRGameMode.h"

#include "StalkerMP/GameModes/BR/Actors/BRZoneActor.h"
#include "StalkerMP/GameModes/BR/Actors/BRLevelDataActor.h"
#include "StalkerMP/GameModes/BR/Logic/BRPlayerController.h"
#include "StalkerMP/GameModes/BR/Logic/BRGameState.h"
#include "StalkerMP/GameModes/BR/Logic/BRPlayerState.h"
#include "StalkerMP/Items/ItemSpawners/ItemSpawner.h"
#include "StalkerMP/Items/DeadBodyBackpack.h"
#include "StalkerMP/Items/BaseItem.h"
#include "StalkerMP/Items/FirearmWeaponItem.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Pawns/Spectator/Spectator.h"
#include "StalkerMP/Levels/WeatherActor.h"
#include "StalkerMP/StalkerMPGameInstance.h"
#include "StalkerMP/SMPFunctions.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"


const FString ABRGameMode::MIN_PLAYERS_TO_START_SETTING_KEY = "BR_MinPlayersToStart";
const FString ABRGameMode::GAME_START_DELAY_SETTING_KEY = "BR_GameStartDelay";
const FString ABRGameMode::POST_FINISH_RESET_DELAY_SETTING_KEY = "BR_PostFinishResetDelay";
const FString ABRGameMode::EARLIEST_START_TIME_SETTING_KEY = "BR_EarliestPossibleTimeOfDayForMatchStart";
const FString ABRGameMode::LATEST_START_TIME_SETTING_KEY = "BR_LatestPossibleTimeOfDayForMatchStart";
const FString ABRGameMode::GAME_TIME_PASS_SETTING_KEY = "BR_TimeOfDayPassUntilZeroSize";


void ABRGameMode::OnGamemodeLevelLoaded()
{
	Super::OnGamemodeLevelLoaded();

	MinPlayersToStart = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, MIN_PLAYERS_TO_START_SETTING_KEY, MinPlayersToStart);
	StartGameDelay = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, GAME_START_DELAY_SETTING_KEY, StartGameDelay);
	RestartGameDelay = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, POST_FINISH_RESET_DELAY_SETTING_KEY, RestartGameDelay);

	EarliestStartTime = USMPFunctions::ParseTimeString(GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, EARLIEST_START_TIME_SETTING_KEY, "07:00:00"));
	LatestStartTime = USMPFunctions::ParseTimeString(GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, LATEST_START_TIME_SETTING_KEY, "16:00:00"));
	GameTimePass = USMPFunctions::ParseTimeString(GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, GAME_TIME_PASS_SETTING_KEY, "3:00:00"));
	GameTimePass = FTimecode(GameTimePass.Hours * 2, GameTimePass.Minutes * 2, GameTimePass.Seconds * 2, 0, false);

	GetGameState<ABRGameState>()->MinPlayersToStart = MinPlayersToStart;

	for (TActorIterator<ABRLevelDataActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABRLevelDataActor* LevelDataActor = *ActorItr;

		ZoneActor = LevelDataActor->ZoneActor;

		break;
	}

	for (TActorIterator<AItemSpawner> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AItemSpawner* ItemSpawner = *ActorItr;

		ItemSpawners.Add(ItemSpawner);
	}

	RestartGame();
}

FString ABRGameMode::GetGameModeName()
{
	return "BR";
}

void ABRGameMode::HandleStartingNewPlayer_Implementation(APlayerController* PlayerController)
{
	Super::HandleStartingNewPlayer_Implementation(PlayerController);

	if (!PlayerStartPIE)
	{
		TArray<AActor*> AllPlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllPlayerStarts);

		FTransform SpawnTransform;
		if (AllPlayerStarts.Num() > 0)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(AllPlayerStarts[UKismetMathLibrary::RandomInteger(AllPlayerStarts.Num())]);
			SpawnTransform = PlayerStart->GetActorTransform();
		}

		RespawnAsPawn((ABRPlayerController*) PlayerController, PlayerSpectatorClass, SpawnTransform);
	}
	else
	{
		RespawnAsPawn((ABRPlayerController*) PlayerController, PlayerSpectatorClass, PlayerStartPIE->GetActorTransform());
	}

	ABRGameState* BRGameState = GetGameState<ABRGameState>();
	if (BRGameState->GetMatchState() == EMatchState::InLobby && BRGameState->PlayerArray.Num() >= MinPlayersToStart)
	{
		LaunchGameStartTimer();
	}
}

void ABRGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// Is last player?
	if (GameState->PlayerArray.Num() < 1)
	{
		RestartGame();
	}
	else if (GameState->PlayerArray.Num() < MinPlayersToStart)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StartCountdownTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(StartCountdownTimerHandle);
			WeatherActor->PauseChangingWeather();

			GetGameState<ABRGameState>()->SetMatchState(EMatchState::InLobby);
		}
	}
}

void ABRGameMode::GrantStartingItems(APlayerCharacter* PlayerCharacter)
{
	for (FBRItemsToGrantOnSpawnList ItemsToGrantOnSpawnList : ItemsToGrantOnSpawn)
	{
		TArray<FBRGrantableItem> ItemsList = ItemsToGrantOnSpawnList.ItemsList;

		if (ItemsList.Num() > 0)
		{
			int ItemNumber = UKismetMathLibrary::RandomInteger(ItemsList.Num());

			FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
			ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(
				ItemsList[ItemNumber].ItemClass,
				PlayerCharacter->GetTransform().GetLocation(),
				FRotator(0, 0, 0),
				SpawnInfo);

			if (ItemsList[ItemNumber].IfWeaponThenFullyLoaded) {
				if (AFirearmWeaponItem* WeaponItem = dynamic_cast<AFirearmWeaponItem*>(SpawnedItem)) {
					WeaponItem->SetAmmo(WeaponItem->GetMaxAmmo());
				}
			}

			PlayerCharacter->Server_PickupItem_Implementation(SpawnedItem);

			for (TSubclassOf<class ABaseItem> SatteliteItemClass : ItemsList[ItemNumber].SatelliteItems)
			{
				SpawnInfo = FActorSpawnParameters();
				ABaseItem* SatelliteItem = GetWorld()->SpawnActor<ABaseItem>(
					SatteliteItemClass,
					PlayerCharacter->GetTransform().GetLocation(),
					FRotator(0, 0, 0),
					SpawnInfo);

				PlayerCharacter->Server_PickupItem_Implementation(SatelliteItem);
			}
		}
	}
}

void ABRGameMode::UpdateGameStartTimer()
{
	GameStartTimer--;
	GetGameState<ABRGameState>()->GameStartTimer = GameStartTimer;

	if (GameStartTimer <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StartCountdownTimerHandle);
		StartGame();
	}
}

void ABRGameMode::LaunchGameStartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(StartCountdownTimerHandle, this, &ABRGameMode::UpdateGameStartTimer, 1, true, 1);

	int EarliestStartTimeInt = USMPFunctions::TimecodeToSeconds(EarliestStartTime);
	int LatestStartTimeInt = USMPFunctions::TimecodeToSeconds(LatestStartTime);

	int NextStartTime = EarliestStartTimeInt + UKismetMathLibrary::RandomInteger(LatestStartTimeInt - EarliestStartTimeInt);
	FTimecode Time = USMPFunctions::SecondsToTimecode(NextStartTime);
	WeatherActor->SetTimeOfDay(Time, StartGameDelay, (NextStartTime - WeatherActor->GetCurrentTime()) < NEXT_START_TIME_MIN_DELTA);

	GetGameState<ABRGameState>()->SetMatchState(EMatchState::Starting);
}

void ABRGameMode::StartGame()
{
	Super::StartGame();

	ZoneActor->Start();

	TArray<ABRPlayerController*> PlayersToSpawn;
	TArray<AActor*> AllJoinedPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABRPlayerController::StaticClass(), AllJoinedPlayers);
	for (AActor* Actor : AllJoinedPlayers)
	{
		ABRPlayerController* BRPlayerController = Cast<ABRPlayerController>(Actor);
		PlayersToSpawn.Add(BRPlayerController);
	}

	// Destroying dead bodies
	for (TActorIterator<ASpectator> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	if (!PlayerStartPIE)
	{
		TArray<AActor*> AllPlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllPlayerStarts);
		int IterationNumber = UKismetMathLibrary::FCeil(1.0 * PlayersToSpawn.Num() / AllPlayerStarts.Num());

		for (int i = 0; i < IterationNumber; i++)
		{
			AllPlayerStarts.Empty();
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllPlayerStarts);

			int PlayersToSpawnOnThisIteration = UKismetMathLibrary::Min(PlayersToSpawn.Num(), AllPlayerStarts.Num());
			for (int j = 0; j < PlayersToSpawnOnThisIteration; j++)
			{
				APlayerStart* PlayerStart = Cast<APlayerStart>(AllPlayerStarts[UKismetMathLibrary::RandomInteger(AllPlayerStarts.Num())]);

				APlayerCharacter* NewCharacter = (APlayerCharacter*) RespawnAsPawn(PlayersToSpawn[0], PlayerCharacterClass, PlayerStart->GetTransform(), false);
				AliveCharacters.Add(NewCharacter);

				GrantStartingItems(NewCharacter);

				PlayersToSpawn[0]->Possess(NewCharacter);
				PlayersToSpawn[0]->ClientSetRotation(PlayerStart->GetTransform().Rotator());

				PlayersToSpawn.RemoveAt(0);
				AllPlayerStarts.Remove(PlayerStart);
			}
		}
	}
	else
	{
		while (PlayersToSpawn.Num() > 0)
		{
			APlayerCharacter* NewCharacter = (APlayerCharacter*) RespawnAsPawn(PlayersToSpawn[0], PlayerCharacterClass, PlayerStartPIE->GetTransform(), false);
			AliveCharacters.Add(NewCharacter);

			GrantStartingItems(NewCharacter);

			PlayersToSpawn[0]->Possess(NewCharacter);
			PlayersToSpawn[0]->ClientSetRotation(PlayerStartPIE->GetTransform().Rotator());

			PlayersToSpawn.RemoveAt(0);
		}
	}

	GetGameState<ABRGameState>()->PlayersAliveCount = AliveCharacters.Num();

	for (AItemSpawner* ItemSpawner : ItemSpawners)
	{
		ItemSpawner->SpawnItems();
	}

	WeatherActor->AddTime(GameTimePass, ZoneActor->GetTimeToFullyShrink() * 2);
}

void ABRGameMode::StopGame()
{
	ZoneActor->Stop();

	Super::StopGame();
}

void ABRGameMode::RestartGame()
{
	Super::RestartGame();

	AliveCharacters.Empty();
	ZoneActor->Reset();
	GameStartTimer = StartGameDelay;
	GetGameState<ABRGameState>()->GameStartTimer = GameStartTimer;

	// Spawning spectators and destroying characters
	for (TActorIterator<APlayerCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerCharacter* PlayerCharacter = *ActorItr;

		if (!PlayerCharacter->GetIsDead())
		{
			if (ABRPlayerController* BRPlayerController = dynamic_cast<ABRPlayerController*>(PlayerCharacter->GetController())) {
				RespawnAsPawn(BRPlayerController, PlayerSpectatorClass, PlayerCharacter->GetCameraTransform());
			}
		}

		PlayerCharacter->Destroy();
	}

	// Destroying dead bodies
	for (TActorIterator<ADeadBodyBackpack> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	// Destroying items
	for (TActorIterator<ABaseItem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	if (GameState->PlayerArray.Num() >= MinPlayersToStart)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(StartCountdownTimerHandle))
		{
			LaunchGameStartTimer();
		}
	}
}

void ABRGameMode::OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser, ADeadBodyBackpack* DeadBodyBackpack)
{
	Super::OnPlayerCharacterDied(PlayerCharacter, Controller, InstigatedBy, DamageCauser, DeadBodyBackpack);

	if (GetGameState<ABRGameState>()->GetMatchState() == EMatchState::Ongoing)
	{
		AliveCharacters.Remove(PlayerCharacter);

		if (Controller)
		{
			if (ABRPlayerController* BRPlayerController = dynamic_cast<ABRPlayerController*>(Controller)) {
				RespawnAsPawn(BRPlayerController, PlayerSpectatorClass, PlayerCharacter->GetCameraTransform());
			}
		}

		GetGameState<ABRGameState>()->PlayersAliveCount = AliveCharacters.Num();

		if (AliveCharacters.Num() <= 1)
		{
			StopGame();
			GetWorld()->GetTimerManager().SetTimer(AfterGameTimerHandle, this, &ABRGameMode::RestartGame, RestartGameDelay, false, RestartGameDelay);
		}
	}
}
