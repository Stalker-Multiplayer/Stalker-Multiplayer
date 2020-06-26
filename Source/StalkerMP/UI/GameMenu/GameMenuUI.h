// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/GameMenu/BaseMenuUI.h"

#include "GameMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UGameMenuUI : public UBaseMenuUI
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintPure)
		FString GetGameVersion();

	UFUNCTION(BlueprintCallable)
		void Resume();

	UFUNCTION(BlueprintCallable)
		void OpenSettings();

	UFUNCTION(BlueprintCallable)
		void ExitToMainMenu();

	UFUNCTION(BlueprintCallable)
		void QuitToDesktop();

	
};
