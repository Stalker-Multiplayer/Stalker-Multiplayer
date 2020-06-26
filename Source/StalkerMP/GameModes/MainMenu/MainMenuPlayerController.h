// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BasePlayerController.h"

#include "MainMenuPlayerController.generated.h"

class UMainMenuUI;
class ULoadingUI;
class UServerBrowserUI;
class USoundConcurrency;

/**
 * 
 */
UCLASS()
class STALKERMP_API AMainMenuPlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

public:

	static const int MAIN_MENU_ZORDER = 0;
	static const int SERVER_BROWSER_ZORDER = 1;
	static const int LOADING_UI_ZORDER = 0;




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnNetworkFailure(ENetworkFailure::Type FailureType, FString ErrorString) override;
	virtual bool IsAnyMenuVisible() override;
	virtual void OnMenuButtonPressed() override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:


	UPROPERTY()
		UMainMenuUI* MainMenuUI;

	UPROPERTY()
		UServerBrowserUI* ServerBrowserUI;

	UPROPERTY()
		ULoadingUI* LoadingUI;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UMainMenuUI> MainMenuUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UServerBrowserUI> ServerBrowserUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<ULoadingUI> LoadingUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* BackgroundAudio;


public:

	UFUNCTION()
		void ConnectToServer(FName ServerAddress);

	UFUNCTION()
		void ShowMainMenu();

	UFUNCTION()
		void ShowServerBrowser();

	UFUNCTION()
		void ShowLoadingScreen();

};
