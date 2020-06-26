// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacterUI.generated.h"

class APlayerCharacter;
class UCrosshairWidget;


UCLASS()
class STALKERMP_API UPlayerCharacterUI : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		class APlayerCharacter* PlayerCharacter;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetHealthPercent();

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetPainStrength();
	
	UFUNCTION(BlueprintPure, Category = "Status")
		int GetCurrentFireMode();

	UFUNCTION(BlueprintPure, Category = "Status")
		int GetCurrentAmmo();

	UFUNCTION(BlueprintPure, Category = "Status")
		int GetAmmoInInventory();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UCrosshairWidget* GetCrosshairWidget();


public:

	UFUNCTION()
		void SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void SetIsDoingLongAction(float LongActionTime, bool TheIsDoingLongAction);

};
