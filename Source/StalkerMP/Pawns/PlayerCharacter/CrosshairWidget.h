// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const int INACCURACY_SCALE = 1000;




// ------------------------------------------------------------------------- Function Override --------------------------------------------------------------------------

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnFloatSettingValueUpdatedDelegate;


	UPROPERTY()
		class APlayerCharacter* PlayerCharacter;

	UPROPERTY()
		float LinesGap = 0;

	UPROPERTY()
		float LinesLength = 10;

	UPROPERTY()
		float LinesThickness = 2;

	UPROPERTY()
		float InacuracyScale = 1;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnFloatSettingUpdated(FString Key, float Value);

	UFUNCTION()
		void SetLinesGap(float TheLinesGap);

	UFUNCTION()
		void SetLinesSize(float Length, float Thickness);

	UFUNCTION()
		void SetInaccuracyScale(float TheInaccuracyScale);


protected:

	UFUNCTION(BlueprintPure, Category = "Weapon")
		float GetInaccuracy();

	UFUNCTION(BlueprintPure, Category = "Weapon")
		EMovingMode GetMovingMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Weapon")
		void OnLinesSizeUpdated(float Length, float Thickness);




public:

	UFUNCTION()
		void SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter) { PlayerCharacter = ThePlayerCharacter; }

};
