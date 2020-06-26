// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

class ALobbyPlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void ToggleReady();


public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void SetPlayersList(const TArray<APlayerState*> &PlayerArray);


};
