// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"

class AMainMenuPlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintPure)
		FString GetGameVersion();


public:

	UFUNCTION(BlueprintCallable)
		void OpenServerBrowser();

	UFUNCTION(BlueprintCallable)
		void OpenSettings();

	UFUNCTION(BlueprintCallable)
		void QuitGame();

};
