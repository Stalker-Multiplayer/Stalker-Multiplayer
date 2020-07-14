// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "BRGameStartingUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UBRGameStartingUI : public UHideableWidget
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void UpdateJoinedPlayersCount(int NewCount, int MinPlayersToStart);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void UpdateGameStartTimer(int NewTimer);

	
};
