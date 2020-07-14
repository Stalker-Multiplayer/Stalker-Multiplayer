// Fill out your copyright notice in the Description page of Project Settings.


#include "HideableWidget.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"

void UHideableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnIntSettingValueUpdatedDelegate.BindUFunction(this, "OnIntSettingUpdated");
	UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.AddUnique(OnIntSettingValueUpdatedDelegate);

	OnIntSettingUpdated(
		ABasePlayerController::HIDE_UI_SETTING_KEY, 
		GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
			ESettingsType::General,
			ABasePlayerController::HIDE_UI_SETTING_KEY,
			ABasePlayerController::DEFAULT_HIDE_UI));
}

void UHideableWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.Remove(OnIntSettingValueUpdatedDelegate);
}

void UHideableWidget::OnIntSettingUpdated(FString Key, int Value)
{
	if (Key.Equals(ABasePlayerController::HIDE_UI_SETTING_KEY))
	{
		if (Value > 0)
		{
			Hide();
		}
		else
		{
			Show();
		}
	}
}

void UHideableWidget::SetVisibility(ESlateVisibility InVisibility)
{
	ActualVisibility = InVisibility;

	if (IsHidden)
	{
		Super::SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Super::SetVisibility(ActualVisibility);
	}
}

void UHideableWidget::Hide()
{
	IsHidden = true;
	Super::SetVisibility(ESlateVisibility::Collapsed);
}

void UHideableWidget::Show()
{
	IsHidden = false;
	Super::SetVisibility(ActualVisibility);
}
