// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterUI.h"

#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Pawns/PlayerCharacter/CrosshairWidget.h"


float UPlayerCharacterUI::GetHealthPercent()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetHealth() / PlayerCharacter->GetMaxHealth();
	}

	return 0;
}

float UPlayerCharacterUI::GetHealth()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetHealth();
	}

	return 0;
}

float UPlayerCharacterUI::GetPainStrength()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetPainStrength();
	}

	return 0;
}

int UPlayerCharacterUI::GetCurrentFireMode()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetCurrentFireMode();
	}

	return 0;
}

int UPlayerCharacterUI::GetCurrentAmmo()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetWeaponAmmo();
	}

	return 0;
}

int UPlayerCharacterUI::GetAmmoInInventory()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetWeaponAmmoInInventory();
	}

	return 0;
}

void UPlayerCharacterUI::SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter)
{
	PlayerCharacter = ThePlayerCharacter;
	GetCrosshairWidget()->SetPlayerCharacter(ThePlayerCharacter);
}