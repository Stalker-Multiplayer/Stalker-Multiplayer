// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/GameMenu/BaseMenuUI.h"

#include "SettingsUI.generated.h"

class UGeneralSettingsUI;
class UVideoSettingsUI;
class USoundSettingsUI;
class UControlsSettingsUI;

/**
 * 
 */
UCLASS()
class STALKERMP_API USettingsUI : public UBaseMenuUI
{
	GENERATED_BODY()





// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	void NativeConstruct();

	virtual void SetPlayerController(ABasePlayerController* ThePlayerController) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

//private:

	


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UGeneralSettingsUI* GetGeneralSettingsUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UVideoSettingsUI* GetVideoSettingsUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		USoundSettingsUI* GetSoundSettingsUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UControlsSettingsUI* GetControlsSettingsUI();

	UFUNCTION(BlueprintCallable)
		void CloseSettings();


public:

	UFUNCTION()
		void HideSubSettings();

	UFUNCTION(BlueprintCallable)
		void ShowGeneralSettingsUI();

	UFUNCTION(BlueprintCallable)
		void ShowVideoSettingsUI();

	UFUNCTION(BlueprintCallable)
		void ShowSoundSettingsUI();

	UFUNCTION(BlueprintCallable)
		void ShowControlsSettingsUI();


};
