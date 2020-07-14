// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "DMGameUI.generated.h"

class ADMPlayerState;
class ADMPlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API UDMGameUI : public UHideableWidget
{
	GENERATED_BODY()
	


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void UpdateTimeLeft(int NewTimeLeft);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void UpdateScoreLimit(int NewScoreLimit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void UpdatePlayersRanks(const TArray<ADMPlayerState*>& Players, int ThisPlayerIndex);

	
};
