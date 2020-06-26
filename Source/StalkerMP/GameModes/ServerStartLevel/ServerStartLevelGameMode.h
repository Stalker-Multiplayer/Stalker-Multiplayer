// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ServerStartLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API AServerStartLevelGameMode : public AGameModeBase
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

public:

	static const FString GAMEMODE_SETTING_KEY;
	static const FString MAPS_SETTING_KEY;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void BeginPlay() override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString DefaultGameMode = FString("");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString DefaultMaps = FString("Develop");


};
