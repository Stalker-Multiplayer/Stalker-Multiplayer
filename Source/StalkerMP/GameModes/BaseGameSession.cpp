// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGameSession.h"

#include "StalkerMP/StalkerMPGameInstance.h"



void ABaseGameSession::RegisterServer()
{
	MaxPlayers = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, UStalkerMPGameInstance::MAX_PLAYERS_SETTING_KEY, UStalkerMPGameInstance::DEFAULT_MAX_PLAYERS);
}

void ABaseGameSession::RegisterServerFailed()
{
	//UE_LOG(LogTemp, Warning, TEXT("Autologin attempt failed, unable to register server!"));
}
