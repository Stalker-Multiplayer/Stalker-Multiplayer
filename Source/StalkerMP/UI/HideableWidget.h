// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HideableWidget.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UHideableWidget : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnIntSettingValueUpdatedDelegate;

	UPROPERTY()
		bool IsHidden = false;

	UPROPERTY()
		ESlateVisibility ActualVisibility = ESlateVisibility::Visible;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void Hide();

	UFUNCTION()
		void Show();


protected:

	UFUNCTION()
		virtual void OnIntSettingUpdated(FString Key, int Value);


};
