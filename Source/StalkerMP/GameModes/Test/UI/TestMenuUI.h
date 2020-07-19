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

	UFUNCTION(BlueprintCallable)
		void OverrideWeatherNormal(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime);

	UFUNCTION(BlueprintCallable)
		void OverrideWeatherSpecial(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime);


public:

	UFUNCTION(BlueprintCallable)
		void SetPlayerController(ATestPlayerController* ThePlayerController) { PlayerController = ThePlayerController; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
		void SetNormalWeatherTypes(const TArray<FString> &WeatherTypes);

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure)
		void SetSpecialWeatherTypes(const TArray<FString> &WeatherTypes);


};
