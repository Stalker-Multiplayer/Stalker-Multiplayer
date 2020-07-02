// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"

#include "StalkerMP/GameModes/Test/Logic/TestPlayerController.h"
#include "StalkerMP/GameModes/BaseGameState.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Pawns/Spectator/Spectator.h"
#include "StalkerMP/Levels/WeatherActor.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"

// Called when the game starts or when spawned
void ATestGameMode::OnGamemodeLevelLoaded()
{
	Super::OnGamemodeLevelLoaded();

	/*for (TActorIterator<ATestLevelDataActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ATestLevelDataActor* LevelDataActor = *ActorItr;

		break;
	}*/

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), AllPlayerStarts);
}

FString ATestGameMode::GetGameModeName()
{
	return "Test";
}

void ATestGameMode::HandleStartingNewPlayer_Implementation(APlayerController* PlayerController)
{
	Super::HandleStartingNewPlayer_Implementation(PlayerController);

	if (!PlayerStartPIE)
	{
		if (AllPlayerStarts.Num() > 0)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(AllPlayerStarts[UKismetMathLibrary::RandomInteger(AllPlayerStarts.Num())]);

			RespawnAsPawn((ATestPlayerController*)PlayerController, DefaultPawnClass, PlayerStart->GetTransform());
		}
		else
		{
			RespawnAsPawn((ATestPlayerController*)PlayerController, DefaultPawnClass, FTransform());
		}
	}
	else
	{
		RespawnAsPawn((ATestPlayerController*)PlayerController, DefaultPawnClass, PlayerStartPIE->GetTransform());
	}
}

void ATestGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (Exiting->GetPawn()) {
		Exiting->GetPawn()->Destroy();
	}
}
