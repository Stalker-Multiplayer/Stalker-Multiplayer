// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/GameMenu/BaseMenuUI.h"

#include "SoundSettingsUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API USoundSettingsUI : public UBaseMenuUI
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	void NativeConstruct() override;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		float NormalizeVolume(float Volume);


protected:

	UFUNCTION(BlueprintCallable)
		void SetMasterVolume(float Volume);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnMasterVolumeUpdated(float NewVolume);

	UFUNCTION(BlueprintCallable)
		void SetGameVolume(float Volume);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnGameVolumeUpdated(float NewVolume);

	UFUNCTION(BlueprintCallable)
		void SetMusicVolume(float Volume);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnMusicVolumeUpdated(float NewVolume);

};
