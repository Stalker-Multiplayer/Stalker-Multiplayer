// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/Actor.h"
#include "ServerData.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UServerData : public UObject
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		FString SessionId;

	UPROPERTY()
		FString ServerName = "";

	UPROPERTY()
		FString GameMode = "";

	UPROPERTY()
		FString MapName = "";

	UPROPERTY()
		FString GameVersion = "";
		
	UPROPERTY()
		int PlayersCount = 0;

	UPROPERTY()
		int MaxPlayersCount = 0;
		
	UPROPERTY()
		int Ping = 0;
	



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void SetSessionId(FString TheSessionId) { SessionId = TheSessionId; }

	UFUNCTION()
		FString GetSessionId() { return SessionId; }

	UFUNCTION()
		void SetServerName(FString TheServerName) { ServerName = TheServerName; }

	UFUNCTION()
		FString GetServerName() { return ServerName; }

	UFUNCTION()
		void SetGameMode(FString TheGameMode) { GameMode = TheGameMode; }

	UFUNCTION()
		FString GetGameMode() { return GameMode; }

	UFUNCTION()
		void SetMapName(FString TheMapName) { MapName = TheMapName; }

	UFUNCTION()
		FString GetGameVersion() { return GameVersion; }

	UFUNCTION()
		void SetGameVersion(FString TheGameVersion) { GameVersion = TheGameVersion; }

	UFUNCTION()
		FString GetMapName() { return MapName; }

	UFUNCTION()
		void SetPlayersCount(int ThePlayersCount) { PlayersCount = ThePlayersCount; }

	UFUNCTION()
		int GetPlayersCount() { return PlayersCount; }

	UFUNCTION()
		void SetMaxPlayersCount(int TheMaxPlayersCount) { MaxPlayersCount = TheMaxPlayersCount; }

	UFUNCTION()
		int GetMaxPlayersCount() { return MaxPlayersCount; }

	UFUNCTION()
		void SetPing(int ThePing) { Ping = ThePing; }

	UFUNCTION()
		int GetPing() { return Ping; }


};
