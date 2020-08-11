// Fill out your copyright notice in the Description page of Project Settings.

#include "DMGameMode.h"

#include "StalkerMP/GameModes/DM/Actors/DMLevelDataActor.h"
#include "StalkerMP/GameModes/DM/Logic/DMPlayerController.h"
#include "StalkerMP/GameModes/DM/Logic/DMGameState.h"
#include "StalkerMP/GameModes/DM/Logic/DMPlayerState.h"
#include "StalkerMP/Items/InventoryComponent.h"
#include "StalkerMP/Items/DeadBodyBackpack.h"
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


const FString ADMGameMode::MIN_PLAYERS_TO_START_SETTING_KEY = "DM_MinPlayersToStart";
const FString ADMGameMode::GAME_START_DELAY_SETTING_KEY = "DM_GameStartDelay";
const FString ADMGameMode::POST_FINISH_RESET_DELAY_SETTING_KEY = "DM_PostFinishResetDelay";
const FString ADMGameMode::TIME_LIMIT = "DM_TimeLimit";
const FString ADMGameMode::SCORE_LIMIT = "DM_ScoreLimit";
const FString ADMGameMode::RESPAWN_DELAY = "DM_RespawnDelay";
const FString ADMGameMode::DROPPED_ITEMS_REMOVE_DELAY = "DM_DroppedItemsRemoveDelay";
const FString ADMGameMode::EARLIEST_START_TIME_SETTING_KEY = "DM_EarliestPossibleTimeOfDayForMatchStart";
const FString ADMGameMode::LATEST_START_TIME_SETTING_KEY = "DM_LatestPossibleTimeOfDayForMatchStart";
const FString ADMGameMode::GAME_TIME_PASS_SETTING_KEY = "DM_TimeOfDayPassDuringMatch";


// Called when the game starts or when spawned
void ADMGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnItemPickedUpDelegate.BindUFunction(this, "OnItemPickedUp");
	UInventoryComponent::OnItemPickedUpDelegate.AddUnique(OnItemPickedUpDelegate);
	OnItemDroppedDelegate.BindUFunction(this, "OnItemDropped");
	UInventoryComponent::OnItemDroppedDelegate.AddUnique(OnItemDroppedDelegate);
}

void ADMGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UInventoryComponent::OnItemPickedUpDelegate.Remove(OnItemPickedUpDelegate);
	UInventoryComponent::OnItemDroppedDelegate.Remove(OnItemDroppedDelegate);
}

void ADMGameMode::OnGamemodeLevelLoaded()
{
	Super::OnGamemodeLevelLoaded();

	MinPlayersToStart = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, MIN_PLAYERS_TO_START_SETTING_KEY, MinPlayersToStart);
	StartGameDelay = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, GAME_START_DELAY_SETTING_KEY, StartGameDelay);
	RestartGameDelay = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, POST_FINISH_RESET_DELAY_SETTING_KEY, RestartGameDelay);
	TimeLimit = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, TIME_LIMIT, TimeLimit);
	ScoreLimit = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, SCORE_LIMIT, ScoreLimit);
	RespawnDelay = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, RESPAWN_DELAY, RespawnDelay);
	DroppedItemsRemoveDelay = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, DROPPED_ITEMS_REMOVE_DELAY, DroppedItemsRemoveDelay);

	if (StartGameDelay < 2)
	{
		StartGameDelay = 2;
	}
	if (RestartGameDelay < 1)
	{
		RestartGameDelay = 1;
	}

	EarliestStartTime = USMPFunctions::ParseTimeString(GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, EARLIEST_START_TIME_SETTING_KEY, "07:00:00"));
	LatestStartTime = USMPFunctions::ParseTimeString(GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, LATEST_START_TIME_SETTING_KEY, "16:00:00"));
	GameTimePass = USMPFunctions::ParseTimeString(GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, GAME_TIME_PASS_SETTING_KEY, "3:00:00"));

	GetGameState<ADMGameState>()->MinPlayersToStart = MinPlayersToStart;

	for (TActorIterator<ADMLevelDataActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ADMLevelDataActor* LevelDataActor = *ActorItr;

		break;
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllPlayerStarts);

	RestartGame();
}

FString ADMGameMode::GetGameModeName()
{
	return "DM";
}

void ADMGameMode::HandleStartingNewPlayer_Implementation(APlayerController* PlayerController)
{
	Super::HandleStartingNewPlayer_Implementation(PlayerController);
	Map_RespawnPlayerTimerHandles.Add(PlayerController, FTimerHandle());

	ADMGameState* DMGameState = GetGameState<ADMGameState>();
	if (DMGameState->GetMatchState() == EMatchState::Ongoing)
	{
		RespawnPlayer((ADMPlayerController*)PlayerController);
	}
	else
	{
		if (!PlayerStartPIE)
		{
			FTransform SpawnTransform;
			if (AllPlayerStarts.Num() > 0)
			{
				APlayerStart* PlayerStart = Cast<APlayerStart>(AllPlayerStarts[UKismetMathLibrary::RandomInteger(AllPlayerStarts.Num())]);
				SpawnTransform = PlayerStart->GetActorTransform();
			}

			RespawnAsPawn((ADMPlayerController*)PlayerController, PlayerSpectatorClass, SpawnTransform);
		}
		else
		{
			RespawnAsPawn((ADMPlayerController*)PlayerController, PlayerSpectatorClass, PlayerStartPIE->GetActorTransform());
		}

		if (DMGameState->PlayerArray.Num() >= MinPlayersToStart)
		{
			if (!GetWorld()->GetTimerManager().IsTimerActive(StartCountdownTimerHandle))
			{
				LaunchGameStartTimer();
			}
		}
	}
}

void ADMGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	GetWorld()->GetTimerManager().ClearTimer(Map_RespawnPlayerTimerHandles[Exiting]);
	Map_RespawnPlayerTimerHandles.Remove(Exiting);

	if (Exiting->GetPawn()) {
		Exiting->GetPawn()->Destroy();
	}

	if (GameState->PlayerArray.Num() < MinPlayersToStart)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StartCountdownTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(StartCountdownTimerHandle);
			WeatherActor->Multicast_PauseChangingWeather();

			GetGameState<ADMGameState>()->SetMatchState(EMatchState::InLobby);
		}

		if (GetGameState<ADMGameState>()->GetMatchState() == EMatchState::Ongoing)
		{
			StopGame();
			GetWorld()->GetTimerManager().SetTimer(AfterGameTimerHandle, this, &ADMGameMode::RestartGame, RestartGameDelay, false, RestartGameDelay);
		}
	}

	// Is last player?
	if (GameState->PlayerArray.Num() < 1)
	{
		RestartGame();
	}
}

void ADMGameMode::GrantStartingItems(APlayerCharacter* PlayerCharacter)
{
	for (FDMItemsToGrantOnSpawnList ItemsToGrantOnSpawnList : ItemsToGrantOnSpawn)
	{
		TArray<FDMGrantableItem> ItemsList = ItemsToGrantOnSpawnList.ItemsList;

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

void ADMGameMode::UpdateGameStartTimer()
{
	GameStartTimer--;
	GetGameState<ADMGameState>()->GameStartTimer = GameStartTimer;

	if (GameStartTimer <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StartCountdownTimerHandle);
		StartGame();
	}
}

void ADMGameMode::LaunchGameStartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(StartCountdownTimerHandle, this, &ADMGameMode::UpdateGameStartTimer, 1, true, 1);

	int EarliestStartTimeInt = USMPFunctions::TimecodeToSeconds(EarliestStartTime);
	int LatestStartTimeInt = USMPFunctions::TimecodeToSeconds(LatestStartTime);

	int NextStartTime = EarliestStartTimeInt + UKismetMathLibrary::RandomInteger(LatestStartTimeInt - EarliestStartTimeInt);
	FTimecode Time = USMPFunctions::SecondsToTimecode(NextStartTime);
	WeatherActor->SetTimeOfDay(Time, StartGameDelay, (NextStartTime - WeatherActor->GetCurrentTime()) < NEXT_START_TIME_MIN_DELTA);

	GetGameState<ADMGameState>()->SetMatchState(EMatchState::Starting);
}

void ADMGameMode::UpdateGameTimer()
{
	TimeLeft--;
	GetGameState<ADMGameState>()->TimeLeft = TimeLeft;

	if (TimeLeft <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
		StopGame();
		GetWorld()->GetTimerManager().SetTimer(AfterGameTimerHandle, this, &ADMGameMode::RestartGame, RestartGameDelay, false, RestartGameDelay);
	}
}

// Called when the game starts or when spawned
void ADMGameMode::StartGame()
{
	Super::StartGame();

	TArray<ADMPlayerController*> PlayersToSpawn;
	TArray<AActor*> AllJoinedPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADMPlayerController::StaticClass(), AllJoinedPlayers);
	for (AActor* Actor : AllJoinedPlayers)
	{
		ADMPlayerController* DMPlayerController = Cast<ADMPlayerController>(Actor);
		PlayersToSpawn.Add(DMPlayerController);
	}

	for (ADMPlayerController* PlayerController : PlayersToSpawn)
	{
		RespawnPlayer(PlayerController);
	}

	TimeLeft = TimeLimit;
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ADMGameMode::UpdateGameTimer, 1, true, 1);

	WeatherActor->AddTime(GameTimePass, TimeLimit);
}

void ADMGameMode::RestartGame()
{
	Super::RestartGame();

	GameStartTimer = StartGameDelay;

	for (const auto& Entry : Map_RespawnPlayerTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(Map_RespawnPlayerTimerHandles[Entry.Key]);
	}

	// Spawning spectators and destroying characters
	for (TActorIterator<APlayerCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerCharacter* PlayerCharacter = *ActorItr;

		if (!PlayerCharacter->GetIsDead())
		{
			if (ADMPlayerController* DMPlayerController = dynamic_cast<ADMPlayerController*>(PlayerCharacter->GetController())) {
				RespawnAsPawn(DMPlayerController, PlayerSpectatorClass, PlayerCharacter->GetCameraTransform());
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

void ADMGameMode::RespawnPlayer(ADMPlayerController* PlayerController)
{
	GetWorld()->GetTimerManager().ClearTimer(Map_RespawnPlayerTimerHandles[PlayerController]);

	if (PlayerController->GetPawn()) {
		PlayerController->GetPawn()->Destroy();
	}

	if (!PlayerStartPIE)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(AllPlayerStarts[UKismetMathLibrary::RandomInteger(AllPlayerStarts.Num())]);

		APlayerCharacter* NewCharacter = (APlayerCharacter*) RespawnAsPawn((ADMPlayerController*)PlayerController, PlayerCharacterClass, PlayerStart->GetTransform(), false);

		GrantStartingItems(NewCharacter);

		PlayerController->Possess(NewCharacter);
		PlayerController->ClientSetRotation(PlayerStart->GetTransform().Rotator());
	}
	else
	{
		APlayerCharacter* NewCharacter = (APlayerCharacter*) RespawnAsPawn((ADMPlayerController*)PlayerController, PlayerCharacterClass, PlayerStartPIE->GetTransform(), false);

		GrantStartingItems(NewCharacter);

		PlayerController->Possess(NewCharacter);
		PlayerController->ClientSetRotation(PlayerStartPIE->GetTransform().Rotator());
	}
}

void ADMGameMode::OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser, ADeadBodyBackpack* DeadBodyBackpack)
{
	Super::OnPlayerCharacterDied(PlayerCharacter, Controller, InstigatedBy, DamageCauser, DeadBodyBackpack);

	if (GetGameState<ADMGameState>()->GetMatchState() == EMatchState::Ongoing)
	{
		if (ADMPlayerController* DMPlayerController = dynamic_cast<ADMPlayerController*>(Controller)) {
			RespawnAsPawn(DMPlayerController, PlayerSpectatorClass, PlayerCharacter->GetCameraTransform());
		}

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("RespawnPlayer"), Controller);
		GetWorld()->GetTimerManager().SetTimer(Map_RespawnPlayerTimerHandles[Controller], TimerDelegate, RespawnDelay, false, RespawnDelay);

		PlayerCharacter->SetLifeSpan(RespawnDelay);
		DeadBodyBackpack->SetLifeSpan(DroppedItemsRemoveDelay);

		if (InstigatedBy && InstigatedBy->GetPlayerState<ADMPlayerState>()->KillCount >= ScoreLimit)
		{
			StopGame();
			GetWorld()->GetTimerManager().SetTimer(AfterGameTimerHandle, this, &ADMGameMode::RestartGame, RestartGameDelay, false, RestartGameDelay);
		}
	}
}

void ADMGameMode::OnItemPickedUp(ABaseItem* PickedItem)
{
	PickedItem->SetLifeSpan(0);
}

void ADMGameMode::OnItemDropped(ABaseItem* DroppedItem)
{
	DroppedItem->SetLifeSpan(DroppedItemsRemoveDelay);
}
