// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerListEntryWidget.h"

#include "StalkerMP/DataClasses/ServerData.h"
#include "StalkerMP/GameModes/MainMenu/ServerBrowserUI.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "OnlineSessionSettings.h"



void UServerListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UServerData* TheServerData = dynamic_cast<UServerData*>(ListItemObject)) {
		ServerData = TheServerData;
	}
}

void UServerListEntryWidget::Connect()
{
	GetGameInstance<UStalkerMPGameInstance>()->JoinServer(ServerData->GetSessionId());
}

FString UServerListEntryWidget::GetServerName()
{
	return ServerData->GetServerName();
}

FString UServerListEntryWidget::GetGameMode()
{
	return ServerData->GetGameMode();
}

FString UServerListEntryWidget::GetMapName()
{
	return ServerData->GetMapName();
}

FString UServerListEntryWidget::GetGameVersion()
{
	return ServerData->GetGameVersion();
}

int UServerListEntryWidget::GetPlayersCount()
{
	return ServerData->GetPlayersCount();
}

int UServerListEntryWidget::GetMaxPlayersCount()
{
	return ServerData->GetMaxPlayersCount();
}

int UServerListEntryWidget::GetPing()
{
	return ServerData->GetPing();
}
