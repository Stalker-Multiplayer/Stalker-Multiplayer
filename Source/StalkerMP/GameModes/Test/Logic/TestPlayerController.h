// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/GameModes/BasePlayerController.h"

#include "TestPlayerController.generated.h"

class UTestMenuUI;
class AWeatherActor;

/**
 * 
 */
UCLASS()
class STALKERMP_API ATestPlayerController : public ABasePlayerController
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const int TEST_MENU_ZORDER = 25;

	static const FName ACTION_SHOW_TEST_MENU;


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual bool IsAnyMenuVisible() override;
	virtual void HideAllUI(bool HideCustom) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UTestMenuUI* TestMenuUI;

	UPROPERTY()
		AWeatherActor* WeatherActor;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UTestMenuUI> TestMenuUIClass;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_RespawnAsPawn(FTransform Transform, TSubclassOf<APawn> PawnClass);


public:

	UFUNCTION()
		void ToggleTestMenu();

	UFUNCTION()
		void RespawnAsPawn(TSubclassOf<APawn> PawnClass);

	UFUNCTION()
		void SetTimeOfDay(FTimecode FinalTime, int SecondsForChange);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetTimeOfDay(FTimecode FinalTime, int SecondsForChange);

	UFUNCTION()
		void AddTime(FTimecode Time, int SecondsForChange);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AddTime(FTimecode Time, int SecondsForChange);

	UFUNCTION()
		void SetAllowedWeathers(const TArray<FString> &AllowedWeathers);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAllowedWeathers(const TArray<FString> &AllowedWeathers);

	UFUNCTION()
		void OverrideWeatherNormal(FString WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_OverrideWeatherNormal(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime);

	UFUNCTION()
		void OverrideWeatherSpecial(FString WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_OverrideWeatherSpecial(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime);


};
