// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPlayerController.h"

#include "StalkerMP/GameModes/Test/Logic/TestGameMode.h"
#include "StalkerMP/GameModes/Test/UI/TestMenuUI.h"
#include "StalkerMP/Levels/WeatherActor.h"

#include "Engine.h"
#include "EngineUtils.h"

const FName ATestPlayerController::ACTION_SHOW_TEST_MENU = "ShowTestMenu";


void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		SetKeyActionBinding(ACTION_SHOW_TEST_MENU, FInputChord(FKey("P"), false, false, false, false), FInputChord());

		TestMenuUI = CreateWidget<UTestMenuUI>(this, TestMenuUIClass);
		TestMenuUI->SetPlayerController(this);
		TestMenuUI->AddToViewport(TEST_MENU_ZORDER);
		TestMenuUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (TActorIterator<AWeatherActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		WeatherActor = *ActorItr;
		if (IsLocalController())
		{
			TestMenuUI->SetNormalWeatherTypes(WeatherActor->GetNormalWeatherTypes());
			TestMenuUI->SetSpecialWeatherTypes(WeatherActor->GetSpecialWeathersTypes());
		}
		break;
	}
}

// Called to bind functionality to input
void ATestPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalController())
	{
		InputComponent->BindAction(ATestPlayerController::ACTION_SHOW_TEST_MENU, IE_Pressed, this, &ATestPlayerController::ToggleTestMenu);
	}
}

void ATestPlayerController::ToggleTestMenu()
{
	if (TestMenuUI)
	{
		if (TestMenuUI->GetVisibility() == ESlateVisibility::Visible) {
			TestMenuUI->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			TestMenuUI->SetVisibility(ESlateVisibility::Visible);
		}
		UpdateMouseControl();
	}
}

bool ATestPlayerController::IsAnyMenuVisible()
{
	if (!Super::IsAnyMenuVisible() && TestMenuUI) {
		return TestMenuUI->GetVisibility() == ESlateVisibility::Visible;
	}

	return Super::IsAnyMenuVisible();
}

void ATestPlayerController::HideAllUI(bool HideCustom)
{
	Super::HideAllUI(HideCustom);

	TestMenuUI->SetVisibility(ESlateVisibility::Collapsed);
	UpdateMouseControl();
}

void ATestPlayerController::RespawnAsPawn(TSubclassOf<APawn> PawnClass)
{
	Server_RespawnAsPawn(PlayerCameraManager->GetActorTransform(), PawnClass);
}

void ATestPlayerController::Server_RespawnAsPawn_Implementation(FTransform Transform, TSubclassOf<APawn> PawnClass)
{
	GetWorld()->GetAuthGameMode<ATestGameMode>()->RespawnAsPawn(this, PawnClass, Transform, true, true);
}

bool ATestPlayerController::Server_RespawnAsPawn_Validate(FTransform Transform, TSubclassOf<APawn> PawnClass)
{
	return true;
}

void ATestPlayerController::SetTimeOfDay(FTimecode FinalTime, int SecondsForChange)
{
	Server_SetTimeOfDay(FinalTime, SecondsForChange);
}

void ATestPlayerController::Server_SetTimeOfDay_Implementation(FTimecode FinalTime, int SecondsForChange)
{
	if (WeatherActor)
	{
		WeatherActor->SetTimeOfDay(FinalTime, SecondsForChange, false);
	}
}

bool ATestPlayerController::Server_SetTimeOfDay_Validate(FTimecode FinalTime, int SecondsForChange)
{
	return true;
}

void ATestPlayerController::AddTime(FTimecode Time, int SecondsForChange)
{
	Server_AddTime(Time, SecondsForChange);
}

void ATestPlayerController::Server_AddTime_Implementation(FTimecode Time, int SecondsForChange)
{
	if (WeatherActor)
	{
		WeatherActor->AddTime(Time, SecondsForChange);
	}
}

bool ATestPlayerController::Server_AddTime_Validate(FTimecode Time, int SecondsForChange)
{
	return true;
}

void ATestPlayerController::SetAllowedWeathers(const TArray<FString> &AllowedWeathers)
{
	Server_SetAllowedWeathers(AllowedWeathers);
}

void ATestPlayerController::Server_SetAllowedWeathers_Implementation(const TArray<FString> &AllowedWeathers)
{
	if (WeatherActor)
	{
		WeatherActor->SetAllowedWeathers(AllowedWeathers);
	}
}

bool ATestPlayerController::Server_SetAllowedWeathers_Validate(const TArray<FString> &AllowedWeathers)
{
	return true;
}

void ATestPlayerController::OverrideWeatherNormal(FString WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	Server_OverrideWeatherNormal(WeatherType, StartTime, StartTimeFull, EndTimeFull, EndTime);
}

void ATestPlayerController::Server_OverrideWeatherNormal_Implementation(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	if (WeatherActor)
	{
		TArray<FString> BetweenWeatherTypes;
		TArray<FTimecode> BetweenStartTimes;
		BetweenStartTimes.Add(StartTimeFull);
		BetweenWeatherTypes.Add(WeatherType);
		BetweenStartTimes.Add(EndTimeFull);
		BetweenWeatherTypes.Add(WeatherType + "");
		WeatherActor->OverrideWeatherNormal(BetweenWeatherTypes, StartTime, BetweenStartTimes, EndTime, true);
	}
}

bool ATestPlayerController::Server_OverrideWeatherNormal_Validate(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	return true;
}

void ATestPlayerController::OverrideWeatherSpecial(FString WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	Server_OverrideWeatherSpecial(WeatherType, StartTime, StartTimeFull, EndTimeFull, EndTime);
}

void ATestPlayerController::Server_OverrideWeatherSpecial_Implementation(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	if (WeatherActor)
	{
		WeatherActor->OverrideWeatherSpecial(WeatherType, StartTime, StartTimeFull, EndTimeFull, EndTime);
	}
}

bool ATestPlayerController::Server_OverrideWeatherSpecial_Validate(const FString &WeatherType, FTimecode StartTime, FTimecode StartTimeFull, FTimecode EndTimeFull, FTimecode EndTime)
{
	return true;
}
