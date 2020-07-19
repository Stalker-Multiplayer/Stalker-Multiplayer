// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMenuUI.h"

#include "StalkerMP/GameModes/Test/Logic/TestPlayerController.h"


void UTestMenuUI::RespawnAsPawn(TSubclassOf<APawn> PawnClass)
{
	PlayerController->RespawnAsPawn(PawnClass);
}

void UTestMenuUI::CloseMenu()
{
	PlayerController->ToggleTestMenu();
}

void UTestMenuUI::SetTimeOfDay(FTimecode FinalTime, int SecondsForChange)
{
	PlayerController->SetTimeOfDay(FinalTime, SecondsForChange);
}

void UTestMenuUI::AddTime(FTimecode Time, int SecondsForChange)
{
	PlayerController->AddTime(Time, SecondsForChange);
}

void UTestMenuUI::SetAllowedWeathers(const TArray<FString> &AllowedWeathers)
{
	PlayerController->SetAllowedWeathers(AllowedWeathers);
}

void UTestMenuUI::OverrideWeatherNormal(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	PlayerController->OverrideWeatherNormal(WeatherType, StartTime, StartTimeFull, EndTimeFull, EndTime);
}

void UTestMenuUI::OverrideWeatherSpecial(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	PlayerController->OverrideWeatherSpecial(WeatherType, StartTime, StartTimeFull, EndTimeFull, EndTime);
}
