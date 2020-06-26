// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundSettingsUI.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"

void USoundSettingsUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetMasterVolume(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::Sound,
		ABasePlayerController::MASTER_VOLUME_SETTING_KEY,
		ABasePlayerController::DEFAULT_MASTER_VOLUME));
	SetGameVolume(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::Sound,
		ABasePlayerController::GAME_VOLUME_SETTING_KEY,
		ABasePlayerController::DEFAULT_GAME_VOLUME));
	SetMusicVolume(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::Sound,
		ABasePlayerController::MUSIC_VOLUME_SETTING_KEY,
		ABasePlayerController::DEFAULT_MUSIC_VOLUME));
}

void USoundSettingsUI::SetMasterVolume(float Volume)
{
	Volume = NormalizeVolume(Volume);
	OnMasterVolumeUpdated(Volume);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::Sound, ABasePlayerController::MASTER_VOLUME_SETTING_KEY, Volume);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Sound);
}

void USoundSettingsUI::SetGameVolume(float Volume)
{
	Volume = NormalizeVolume(Volume);
	OnGameVolumeUpdated(Volume);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::Sound, ABasePlayerController::GAME_VOLUME_SETTING_KEY, Volume);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Sound);
}

void USoundSettingsUI::SetMusicVolume(float Volume)
{
	Volume = NormalizeVolume(Volume);
	OnMusicVolumeUpdated(Volume);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::Sound, ABasePlayerController::MUSIC_VOLUME_SETTING_KEY, Volume);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Sound);
}

float USoundSettingsUI::NormalizeVolume(float Volume)
{
	if (Volume < 0)
	{
		Volume = 0;
	}

	return Volume;
}
