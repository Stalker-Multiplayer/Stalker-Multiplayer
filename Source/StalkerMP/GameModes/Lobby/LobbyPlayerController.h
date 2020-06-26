// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BasePlayerController.h"

#include "LobbyPlayerController.generated.h"

class ULobbyUI;
class ULoadingUI;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class STALKERMP_API ALobbyPlayerController : public ABasePlayerController
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

public:

	static const int LOBBY_UI_ZORDER = 0;
	static const int LOADING_UI_ZORDER = 0;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	// HUD

	UPROPERTY()
		class ULobbyUI* LobbyUI;

	UPROPERTY()
		class ULoadingUI* LoadingUI;


	// Sound

	UPROPERTY()
		FTimerHandle GuitarTimerHandle;


protected:

	// HUD

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<class ULobbyUI> LobbyUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<class ULoadingUI> LoadingUIClass;


	// Sound

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* BackgroundAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<USoundBase*> GuitarTracks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FIntPoint MinMaxGuitarDelay = FIntPoint(5, 10);




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void WaitToPlayGuitar();

	UFUNCTION()
		void PlayRandomGuitarTrack();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_ToggleReady();


public:

	UFUNCTION(Client, Reliable)
		void Client_NotifyLoading();

	UFUNCTION()
		void ToggleReady();

};
