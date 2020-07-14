// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "BRGameUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UBRGameUI : public UHideableWidget
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------


public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void UpdatePlayersAliveCount(int NewCount);

	
};
