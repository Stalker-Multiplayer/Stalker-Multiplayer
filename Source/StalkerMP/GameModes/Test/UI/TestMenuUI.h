// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestMenuUI.generated.h"

class ATestPlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API UTestMenuUI : public UUserWidget
{
	GENERATED_BODY()
	

// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		ATestPlayerController* PlayerController;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void RespawnAsPawn(TSubclassOf<APawn> PawnClass);

	UFUNCTION(BlueprintCallable)
		void CloseMenu();

	UFUNCTION(BlueprintCallable)
		void SetTimeOfDay(FTimecode FinalTime, int SecondsForChange);

	UFUNCTION(BlueprintCallable)
		void AddTime(FTimecode Time, int SecondsForChange);

	UFUNCTION(BlueprintCallable)
		void SetAllowedWeathers(const TArray<FString> &AllowedWeathers);


public:

	UFUNCTION(BlueprintCallable)
		void SetPlayerController(ATestPlayerController* ThePlayerController) { PlayerController = ThePlayerController; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
		void SetWeatherNames(const TArray<FString> &WeatherNames);


};
