// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerStartLevelGameMode.h"

#include "StalkerMP/StalkerMPGameInstance.h"

#include "Kismet/GameplayStatics.h"


const FString AServerStartLevelGameMode::GAMEMODE_SETTING_KEY = "GameMode";
const FString AServerStartLevelGameMode::MAPS_SETTING_KEY = "Maps";


// Called when the game starts or when spawned
void AServerStartLevelGameMode::BeginPlay()
{
	FString GameMode = GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, GAMEMODE_SETTING_KEY, DefaultGameMode);

	FString LevelsSetting = GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(ESettingsType::Server, MAPS_SETTING_KEY, DefaultMaps);
	FString StartingLevel = LevelsSetting;
	LevelsSetting = LevelsSetting.Replace(TEXT(" "), TEXT(""));
	LevelsSetting.Split(",", &StartingLevel, &LevelsSetting);
	FString GamemodeLevel = StartingLevel + "_" + GameMode;

	FString Options;
	if (!DefaultGameMode.Equals(""))
	{
		Options = "Game=/Game/GameModes/{GM}/Logic/{GM}GameModeBP.{GM}GameModeBP_C";
		Options = Options.Replace(TEXT("{GM}"), *GameMode);
	}

	UGameplayStatics::OpenLevel(this, FName(*StartingLevel), true, Options);
}
