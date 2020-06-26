// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ServerListEntryWidget.generated.h"

class UServerData;

/**
 * 
 */
UCLASS()
class STALKERMP_API UServerListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
	


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	void NativeOnListItemObjectSet(UObject* ListItemObject) override;





// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UServerData* ServerData;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void Connect();

	UFUNCTION(BlueprintPure)
		FString GetServerName();

	UFUNCTION(BlueprintPure)
		FString GetGameMode();

	UFUNCTION(BlueprintPure)
		FString GetMapName();

	UFUNCTION(BlueprintPure)
		FString GetGameVersion();

	UFUNCTION(BlueprintPure)
		int GetPlayersCount();

	UFUNCTION(BlueprintPure)
		int GetMaxPlayersCount();

	UFUNCTION(BlueprintPure)
		int GetPing();



};
