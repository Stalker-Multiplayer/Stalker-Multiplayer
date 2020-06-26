// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "LobbyPlayerListEntryWidget.generated.h"

class ALobbyPlayerState;

/**
 * 
 */
UCLASS()
class STALKERMP_API ULobbyPlayerListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
	


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;





// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		ALobbyPlayerState* LobbyPlayerState;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintPure)
		FString GetPlayerName();

	UFUNCTION(BlueprintPure)
		bool GetIsPlayerReady();



};
