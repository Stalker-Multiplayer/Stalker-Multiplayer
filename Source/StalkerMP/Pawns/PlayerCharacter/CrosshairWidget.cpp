// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"


void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetLinesGap(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::CROSSHAIR_LINES_GAP_SETTING_KEY,
		ABasePlayerController::DEFAULT_CROSSHAIR_LINES_GAP));

	SetLinesSize(
		GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
			ESettingsType::General,
			ABasePlayerController::CROSSHAIR_LINES_LENGTH_SETTING_KEY,
			ABasePlayerController::DEFAULT_CROSSHAIR_LINES_LENGTH),
		GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
			ESettingsType::General,
			ABasePlayerController::CROSSHAIR_LINES_THICKNESS_SETTING_KEY,
			ABasePlayerController::DEFAULT_CROSSHAIR_LINES_THICKNESS));

	SetInaccuracyScale(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::General,
		ABasePlayerController::CROSSHAIR_INACCURACY_SCALE_SETTING_KEY,
		ABasePlayerController::DEFAULT_CROSSHAIR_INACCURACY_SCALE));

	OnFloatSettingValueUpdatedDelegate.BindUFunction(this, "OnFloatSettingUpdated");
	UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.AddUnique(OnFloatSettingValueUpdatedDelegate);
}

void UCrosshairWidget::NativeDestruct()
{
	UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.Remove(OnFloatSettingValueUpdatedDelegate);
}

float UCrosshairWidget::GetInaccuracy()
{
	if (PlayerCharacter)
	{
		return LinesGap + PlayerCharacter->GetWeaponInaccuracyRad() * InacuracyScale * INACCURACY_SCALE;
	}
	else
	{
		return 0;
	}
}

EMovingMode UCrosshairWidget::GetMovingMode()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetMovingMode();
	}
	else
	{
		return EMovingMode::Running;
	}
}

void UCrosshairWidget::SetLinesGap(float TheLinesGap)
{
	LinesGap = TheLinesGap;
}

void UCrosshairWidget::SetLinesSize(float Length, float Thickness)
{
	LinesLength = Length;
	LinesThickness = Thickness;
	OnLinesSizeUpdated(LinesLength, LinesThickness);
}

void UCrosshairWidget::SetInaccuracyScale(float TheInacuracyScale)
{
	InacuracyScale = TheInacuracyScale;
}

void UCrosshairWidget::OnFloatSettingUpdated(FString Key, float Value)
{
	if (Key.Equals(ABasePlayerController::CROSSHAIR_LINES_GAP_SETTING_KEY))
	{
		SetLinesGap(Value);
	}
	else if (Key.Equals(ABasePlayerController::CROSSHAIR_LINES_LENGTH_SETTING_KEY))
	{
		SetLinesSize(Value, LinesThickness);
	}
	else if (Key.Equals(ABasePlayerController::CROSSHAIR_LINES_THICKNESS_SETTING_KEY))
	{
		SetLinesSize(LinesLength, Value);
	}
	else if (Key.Equals(ABasePlayerController::CROSSHAIR_INACCURACY_SCALE_SETTING_KEY))
	{
		SetInaccuracyScale(Value);
	}
}
