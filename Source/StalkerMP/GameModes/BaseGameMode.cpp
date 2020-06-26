// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameMode.h"

#include "StalkerMP/StalkerMPGameInstance.h"
#include "StalkerMP/DataClasses/BaseLevelDataActor.h"
#include "StalkerMP/GameModes/BaseGameSession.h"
#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/GameModes/BaseGameState.h"
#include "StalkerMP/GameModes/BasePlayerState.h"
#include "StalkerMP/Pawns/Spectator/Spectator.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Items/BulletProjectile.h"
#include "StalkerMP/Levels/WeatherActor.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"


ABaseGameMode::ABaseGameMode()
{
	bUseSeamlessTravel = true;
	bPauseable = false;
}

// Called when the game starts or when spawned
void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance<UStalkerMPGameInstance>()->UpdateSession(GetGameModeName(), UGameplayStatics::GetCurrentLevelName(GetWorld(), true));

	for (TActorIterator<APlayerStartPIE> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		PlayerStartPIE = *ActorItr;

		break;
	}

	for (TActorIterator<AWeatherActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		WeatherActor = *ActorItr;
		break;
	}

	OnPlayerCharacterDiedDelegate.BindUFunction(this, "OnPlayerCharacterDied");
	APlayerCharacter::OnPlayerCharacterDiedDelegate.AddUnique(OnPlayerCharacterDiedDelegate);
	OnGamemodeLevelLoadedDelegate.BindUFunction(this, "OnGamemodeLevelLoaded");
	ABaseLevelDataActor::GamemodeLevelLoadedDelegate.AddUnique(OnGamemodeLevelLoadedDelegate);

	FString GamemodeLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true) + "_" + GetGameModeName();
	UGameplayStatics::LoadStreamLevel(GetWorld(), FName(*GamemodeLevelName), true, true, FLatentActionInfo());
}

void ABaseGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	APlayerCharacter::OnPlayerCharacterDiedDelegate.Remove(OnPlayerCharacterDiedDelegate);
	ABaseLevelDataActor::GamemodeLevelLoadedDelegate.Remove(OnGamemodeLevelLoadedDelegate);
}

void ABaseGameMode::HandleStartingNewPlayer_Implementation(APlayerController* PlayerController)
{
	//Super::HandleStartingNewPlayer_Implementation(PlayerController);

	GetGameState<ABaseGameState>()->SetJoinedPlayersCount(GameState->PlayerArray.Num());
}

void ABaseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (ABasePlayerController* PlayerController = dynamic_cast<ABasePlayerController*>(Exiting))
	{
		GetGameState<ABaseGameState>()->RemovePlayerState(PlayerController->GetPlayerState<ABasePlayerState>());
		GetGameState<ABaseGameState>()->SetJoinedPlayersCount(GameState->PlayerArray.Num());
	}
}

ASpectator* ABaseGameMode::SpawnSpectator(FTransform Transform)
{
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
	return GetWorld()->SpawnActor<ASpectator>(
		PlayerSpectatorClass,
		Transform,
		SpawnInfo);
}

APlayerCharacter* ABaseGameMode::SpawnPlayerCharacter(FTransform Transform)
{
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	return GetWorld()->SpawnActor<APlayerCharacter>(
		PlayerCharacterClass,
		Transform.GetLocation(),
		FRotator(0, Transform.GetRotation().Rotator().Yaw, 0),
		SpawnInfo);
}

void ABaseGameMode::OnGamemodeLevelLoaded()
{
	ABaseLevelDataActor::GamemodeLevelLoadedDelegate.Remove(OnGamemodeLevelLoadedDelegate);
}

FString ABaseGameMode::GetGameModeName()
{
	return "None";
}

void ABaseGameMode::StartGame()
{
	for (APlayerState* PlayerState : GetGameState<ABaseGameState>()->PlayerArray)
	{
		((ABasePlayerState*)PlayerState)->KillCount = 0;
		((ABasePlayerState*)PlayerState)->DeathCount = 0;
	}

	// Destroying spectators
	for (TActorIterator<ASpectator> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	GetGameState<ABaseGameState>()->SetMatchState(EMatchState::Ongoing);
}

void ABaseGameMode::StopGame()
{
	GetGameState<ABaseGameState>()->SetMatchState(EMatchState::Ended);

	// Destroying bullets
	for (TActorIterator<ABulletProjectile> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}
}

void ABaseGameMode::RestartGame()
{
	StopGame();
	GetGameState<ABaseGameState>()->SetMatchState(EMatchState::InLobby);
}

void ABaseGameMode::OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser, ADeadBodyBackpack* DeadBodyBackpack)
{
	if (InstigatedBy)
	{
		InstigatedBy->GetPlayerState<ABasePlayerState>()->KillCount++;
	}
	if (Controller)
	{
		Controller->GetPlayerState<ABasePlayerState>()->DeathCount++;
	}
}
