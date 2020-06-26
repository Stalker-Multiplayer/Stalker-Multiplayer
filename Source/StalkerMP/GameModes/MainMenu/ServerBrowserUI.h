// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerBrowserUI.generated.h"

class AMainMenuPlayerController;
class UServerData;

/**
 * 
 */
UCLASS()
class STALKERMP_API UServerBrowserUI : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------


public:

	FDelegateHandle OnSessionsFoundDelegateHandle;
	FDelegateHandle OnSessionsFindCanceledDelegateHandle;
	FDelegateHandle OnSessionJoinCompleteDelegateHandle;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnServersFound(TArray<UServerData*> Servers);

	UFUNCTION()
		void OnFindServersCanceled(bool Successful);

	// For some reason EOnJoinSessionCompleteResult isn't visible. Have to use Lambda instead
	/*UFUNCTION()
		void OnJoinSessionComplete(FName SessionName, EOnJoinSessionResult Result);*/


protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void SetServersList(const TArray<UServerData*> &ServerArray);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void ShowProgressMessage(bool Show, const FText &Message);

	UFUNCTION(BlueprintCallable)
		void CancelCurrentProgress();

	UFUNCTION(BlueprintCallable)
		void CloseServerBrowser();

	UFUNCTION(BlueprintCallable)
		void CloseErrorMessage();



public:

	UFUNCTION(BlueprintCallable)
		void RefreshList();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void ShowErrorMessage(bool Show, const FText &Message);

	UFUNCTION(BlueprintCallable)
		void ConnectToServer(FName ServerAddress);

};
