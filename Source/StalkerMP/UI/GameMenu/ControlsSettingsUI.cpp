// Fill out your copyright notice in the Description page of Project Settings.

#include "ControlsSettingsUI.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"


void UControlsSettingsUI::NativeConstruct()
{
	Super::NativeConstruct();

	IsBusySettingKeys = true;

	SetHorizontalSensitivity(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::Controls,
		ABasePlayerController::HORIZONTAL_SENSITIVITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_HORIZONTAL_SENSITIVITY));
	SetVerticalSensitivity(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
		ESettingsType::Controls,
		ABasePlayerController::VERTICAL_SENSITIVITY_SETTING_KEY,
		ABasePlayerController::DEFAULT_VERTICAL_SENSITIVITY));

	// Movement
	ReadAndApplyKeySetting(EPlayerAction::MoveForward);
	ReadAndApplyKeySetting(EPlayerAction::MoveBackward);
	ReadAndApplyKeySetting(EPlayerAction::StrafeLeft);
	ReadAndApplyKeySetting(EPlayerAction::StrafeRight);
	ReadAndApplyKeySetting(EPlayerAction::Jump);
	ReadAndApplyKeySetting(EPlayerAction::Crouch);
	ReadAndApplyKeySetting(EPlayerAction::Sprint);
	ReadAndApplyKeySetting(EPlayerAction::Walk);
	
	// Firing
	ReadAndApplyKeySetting(EPlayerAction::FirePrimary);
	ReadAndApplyKeySetting(EPlayerAction::FireSecondary);
	ReadAndApplyKeySetting(EPlayerAction::Reload);
	ReadAndApplyKeySetting(EPlayerAction::SwitchFireMode);

	// Inventory
	ReadAndApplyKeySetting(EPlayerAction::ToggleInventory);
	ReadAndApplyKeySetting(EPlayerAction::PickWeapon1);
	ReadAndApplyKeySetting(EPlayerAction::PickWeapon2);
	ReadAndApplyKeySetting(EPlayerAction::PickWeapon3);
	ReadAndApplyKeySetting(EPlayerAction::UseBandage);
	ReadAndApplyKeySetting(EPlayerAction::UseFirstAidKit);

	//Misc
	ReadAndApplyKeySetting(EPlayerAction::UseAction);
	ReadAndApplyKeySetting(EPlayerAction::ToggleMap);

	IsBusySettingKeys = false;
}

FString UControlsSettingsUI::PlayerActionToConfigName(EPlayerAction PlayerAction, bool IsSecondary)
{
	FString ConfigName = "";

	switch (PlayerAction)
	{
	// Movement
	case EPlayerAction::MoveForward: ConfigName = ABasePlayerController::ACTION_MOVE_FORWARD.ToString(); break;
	case EPlayerAction::MoveBackward: ConfigName = ABasePlayerController::ACTION_MOVE_BACKWARD.ToString(); break;
	case EPlayerAction::StrafeLeft: ConfigName = ABasePlayerController::ACTION_STRAFE_LEFT.ToString(); break;
	case EPlayerAction::StrafeRight: ConfigName = ABasePlayerController::ACTION_STRAFE_RIGHT.ToString(); break;
	case EPlayerAction::Sprint: ConfigName = ABasePlayerController::ACTION_SPRINT.ToString(); break;
	case EPlayerAction::Walk: ConfigName = ABasePlayerController::ACTION_WALK.ToString(); break;
	case EPlayerAction::Jump: ConfigName = ABasePlayerController::ACTION_JUMP.ToString(); break;
	case EPlayerAction::Crouch: ConfigName = ABasePlayerController::ACTION_CROUCH.ToString(); break;

	// Firing
	case EPlayerAction::FirePrimary: ConfigName = ABasePlayerController::ACTION_FIRE_PRIMARY.ToString(); break;
	case EPlayerAction::FireSecondary: ConfigName = ABasePlayerController::ACTION_FIRE_SECONDARY.ToString(); break;
	case EPlayerAction::Reload: ConfigName = ABasePlayerController::ACTION_RELOAD.ToString(); break;
	case EPlayerAction::SwitchFireMode: ConfigName = ABasePlayerController::ACTION_SWITCH_FIRE_MODE.ToString(); break;

	// Inventory
	case EPlayerAction::ToggleInventory: ConfigName = ABasePlayerController::ACTION_TOGGLE_INVENTORY.ToString(); break;
	case EPlayerAction::PickWeapon1: ConfigName = ABasePlayerController::ACTION_PICK_WEAPON_1.ToString(); break;
	case EPlayerAction::PickWeapon2: ConfigName = ABasePlayerController::ACTION_PICK_WEAPON_2.ToString(); break;
	case EPlayerAction::PickWeapon3: ConfigName = ABasePlayerController::ACTION_PICK_WEAPON_3.ToString(); break;
	case EPlayerAction::UseBandage: ConfigName = ABasePlayerController::ACTION_USE_BANDAGE.ToString(); break;
	case EPlayerAction::UseFirstAidKit: ConfigName = ABasePlayerController::ACTION_USE_FIRST_AID_KIT.ToString(); break;

	// Misc
	case EPlayerAction::UseAction: ConfigName = ABasePlayerController::ACTION_ACTION.ToString(); break;
	case EPlayerAction::ToggleMap: ConfigName = ABasePlayerController::ACTION_TOGGLE_MAP.ToString(); break;
	}

	if (IsSecondary)
	{
		ConfigName = ConfigName + "Secondary";
	}

	return ConfigName;
}

FString UControlsSettingsUI::DefaultPlayerActionKey(EPlayerAction PlayerAction, bool IsSecondary)
{
	switch (PlayerAction)
	{
		// Movement
	case EPlayerAction::MoveForward: if (!IsSecondary) return DEFAULT_MOVE_FORWARD; else return DEFAULT_MOVE_FORWARD_SECONDARY;
	case EPlayerAction::MoveBackward: if (!IsSecondary) return DEFAULT_MOVE_BACKWARD; else return DEFAULT_MOVE_BACKWARD_SECONDARY;
	case EPlayerAction::StrafeLeft: if (!IsSecondary) return DEFAULT_STRAFE_LEFT; else return DEFAULT_STRAFE_LEFT_SECONDARY;
	case EPlayerAction::StrafeRight: if (!IsSecondary) return DEFAULT_STRAFE_RIGHT; else return DEFAULT_STRAFE_RIGHT_SECONDARY;
	case EPlayerAction::Sprint: if (!IsSecondary) return DEFAULT_SPRINT; else return DEFAULT_SPRINT_SECONDARY;
	case EPlayerAction::Walk: if (!IsSecondary) return DEFAULT_WALK; else return DEFAULT_WALK_SECONDARY;
	case EPlayerAction::Jump: if (!IsSecondary) return DEFAULT_JUMP; else return DEFAULT_JUMP_SECONDARY;
	case EPlayerAction::Crouch: if (!IsSecondary) return DEFAULT_CROUCH; else return DEFAULT_CROUCH_SECONDARY;

		// Firing
	case EPlayerAction::FirePrimary: if (!IsSecondary) return DEFAULT_FIRE_PRIMARY; else return DEFAULT_FIRE_PRIMARY_SECONDARY;
	case EPlayerAction::FireSecondary: if (!IsSecondary) return DEFAULT_FIRE_SECONDARY; else return DEFAULT_FIRE_SECONDARY_SECONDARY;
	case EPlayerAction::Reload: if (!IsSecondary) return DEFAULT_RELOAD; else return DEFAULT_RELOAD_SECONDARY;
	case EPlayerAction::SwitchFireMode: if (!IsSecondary) return DEFAULT_SWITCH_FIRE_MODE; else return DEFAULT_SWITCH_FIRE_MODE_SECONDARY;

		// Inventory
	case EPlayerAction::ToggleInventory: if (!IsSecondary) return DEFAULT_TOGGLE_INVENTORY; else return DEFAULT_TOGGLE_INVENTORY_SECONDARY;
	case EPlayerAction::PickWeapon1: if (!IsSecondary) return DEFAULT_PICK_WEAPON_1; else return DEFAULT_PICK_WEAPON_1_SECONDARY;
	case EPlayerAction::PickWeapon2: if (!IsSecondary) return DEFAULT_PICK_WEAPON_2; else return DEFAULT_PICK_WEAPON_2_SECONDARY;
	case EPlayerAction::PickWeapon3: if (!IsSecondary) return DEFAULT_PICK_WEAPON_3; else return DEFAULT_PICK_WEAPON_3_SECONDARY;
	case EPlayerAction::UseBandage: if (!IsSecondary) return DEFAULT_USE_BANDAGE; else return DEFAULT_USE_BANDAGE_SECONDARY;
	case EPlayerAction::UseFirstAidKit: if (!IsSecondary) return DEFAULT_USE_FIRST_AID_KIT; else return DEFAULT_USE_FIRST_AID_KIT_SECONDARY;

		// Misc
	case EPlayerAction::UseAction: if (!IsSecondary) return DEFAULT_ACTION; else return DEFAULT_ACTION_SECONDARY;
	case EPlayerAction::ToggleMap: if (!IsSecondary) return DEFAULT_TOGGLE_MAP; else return DEFAULT_TOGGLE_MAP_SECONDARY;
	}

	return "";
}

void UControlsSettingsUI::PlayerActionToActionNames(EPlayerAction PlayerAction, TArray<FName> &ActionNames)
{
	switch (PlayerAction)
	{
	// Movement
	case EPlayerAction::Sprint:
		ActionNames.Add(ABasePlayerController::ACTION_SPRINT);
		break;
	case EPlayerAction::Walk:
		ActionNames.Add(ABasePlayerController::ACTION_WALK);
		break;
	case EPlayerAction::Jump:
		ActionNames.Add(ABasePlayerController::ACTION_JUMP);
		break;
	case EPlayerAction::Crouch:
		ActionNames.Add(ABasePlayerController::ACTION_CROUCH);
		break;

	// Firing
	case EPlayerAction::FirePrimary:
		ActionNames.Add(ABasePlayerController::ACTION_FIRE_PRIMARY);
		break;
	case EPlayerAction::FireSecondary:
		ActionNames.Add(ABasePlayerController::ACTION_FIRE_SECONDARY);
		break;
	case EPlayerAction::Reload:
		ActionNames.Add(ABasePlayerController::ACTION_RELOAD);
		break;
	case EPlayerAction::SwitchFireMode:
		ActionNames.Add(ABasePlayerController::ACTION_SWITCH_FIRE_MODE);
		break;

	// Inventory
	case EPlayerAction::ToggleInventory:
		ActionNames.Add(ABasePlayerController::ACTION_TOGGLE_INVENTORY);
		break;
	case EPlayerAction::PickWeapon1:
		ActionNames.Add(ABasePlayerController::ACTION_PICK_WEAPON_1);
		break;
	case EPlayerAction::PickWeapon2:
		ActionNames.Add(ABasePlayerController::ACTION_PICK_WEAPON_2);
		break;
	case EPlayerAction::PickWeapon3:
		ActionNames.Add(ABasePlayerController::ACTION_PICK_WEAPON_3);
		break;
	case EPlayerAction::UseBandage:
		ActionNames.Add(ABasePlayerController::ACTION_USE_BANDAGE);
		break;
	case EPlayerAction::UseFirstAidKit:
		ActionNames.Add(ABasePlayerController::ACTION_USE_FIRST_AID_KIT);
		break;

	// Misc
	case EPlayerAction::UseAction:
		ActionNames.Add(ABasePlayerController::ACTION_ACTION);
		break;
	case EPlayerAction::ToggleMap:
		ActionNames.Add(ABasePlayerController::ACTION_TOGGLE_MAP);
		break;
	}
}

void UControlsSettingsUI::PlayerActionToAxisNames(EPlayerAction PlayerAction, TArray<FName> &AxisNames, TArray<int> &AxisValues)
{
	switch (PlayerAction)
	{
		// Movement
	case EPlayerAction::MoveForward:
		AxisNames.Add(ABasePlayerController::ACTION_MOVE_FORWARD);
		AxisValues.Add(1);
		break;
	case EPlayerAction::MoveBackward:
		AxisNames.Add(ABasePlayerController::ACTION_MOVE_BACKWARD);
		AxisValues.Add(-1);
		break;
	case EPlayerAction::StrafeLeft:
		AxisNames.Add(ABasePlayerController::ACTION_STRAFE_LEFT);
		AxisValues.Add(-1);
		break;
	case EPlayerAction::StrafeRight:
		AxisNames.Add(ABasePlayerController::ACTION_STRAFE_RIGHT);
		AxisValues.Add(1);
		break;
	case EPlayerAction::Jump:
		AxisNames.Add(ABasePlayerController::ACTION_JUMP);
		AxisValues.Add(1);
		break;
	case EPlayerAction::Crouch:
		AxisNames.Add(ABasePlayerController::ACTION_CROUCH);
		AxisValues.Add(-1);
		break;
	}
}

void UControlsSettingsUI::SetHorizontalSensitivity(float Sensitivity)
{
	OnHorizontalSensitivityUpdated(Sensitivity);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::Controls, ABasePlayerController::HORIZONTAL_SENSITIVITY_SETTING_KEY, Sensitivity);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Controls);
}

void UControlsSettingsUI::SetVerticalSensitivity(float Sensitivity)
{
	OnVerticalSensitivityUpdated(Sensitivity);

	GetGameInstance<UStalkerMPGameInstance>()->PutFloatSettingValue(ESettingsType::Controls, ABasePlayerController::VERTICAL_SENSITIVITY_SETTING_KEY, Sensitivity);
	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Controls);
}

void UControlsSettingsUI::ReadAndApplyKeySetting(EPlayerAction PlayerAction)
{

	FString KeyConfigValue = GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(
		ESettingsType::Controls,
		PlayerActionToConfigName(PlayerAction, false),
		DefaultPlayerActionKey(PlayerAction, false));
	FString KeyName;
	FString WithShift;
	FString WithCtrl;
	FString WithAlt;
	FString WithCmd;
	KeyConfigValue.Split(",", &KeyName, &KeyConfigValue);
	KeyConfigValue.Split(",", &WithShift, &KeyConfigValue);
	KeyConfigValue.Split(",", &WithCtrl, &KeyConfigValue);
	KeyConfigValue.Split(",", &WithAlt, &WithCmd);
	FInputChord InputChord_primary(FKey(*KeyName), WithShift.ToBool(), WithCtrl.ToBool(), WithAlt.ToBool(), WithCmd.ToBool());

	FString KeyConfigValue2 = GetGameInstance<UStalkerMPGameInstance>()->GetStringSettingValue(
		ESettingsType::Controls,
		PlayerActionToConfigName(PlayerAction, true),
		DefaultPlayerActionKey(PlayerAction, true));
	FString KeyName2;
	FString WithShift2;
	FString WithCtrl2;
	FString WithAlt2;
	FString WithCmd2;
	KeyConfigValue2.Split(",", &KeyName2, &KeyConfigValue2);
	KeyConfigValue2.Split(",", &WithShift2, &KeyConfigValue2);
	KeyConfigValue2.Split(",", &WithCtrl2, &KeyConfigValue2);
	KeyConfigValue2.Split(",", &WithAlt2, &WithCmd2);
	FInputChord InputChord_secondary(FKey(*KeyName2), WithShift2.ToBool(), WithCtrl2.ToBool(), WithAlt2.ToBool(), WithCmd2.ToBool());

	SetKeyBinding(PlayerAction, InputChord_primary, InputChord_secondary);
}

void UControlsSettingsUI::SetKeyBinding(EPlayerAction PlayerAction, FInputChord InputChord_primary, FInputChord InputChord_secondary)
{
	if (InputChord_primary.Key == InputChord_secondary.Key)
	{
		InputChord_secondary = FInputChord();
	}

	TArray<FName> ActionNames;
	PlayerActionToActionNames(PlayerAction, ActionNames);

	for (int i = 0; i < ActionNames.Num(); i++)
	{
		FName ActionName = ActionNames[i];

		PlayerController->SetKeyActionBinding(ActionName, InputChord_primary, InputChord_secondary);
	}

	TArray<FName> AxisNames;
	TArray<int> AxisValues;
	PlayerActionToAxisNames(PlayerAction, AxisNames, AxisValues);

	for (int i = 0; i < AxisNames.Num(); i++)
	{
		FName ActionName = AxisNames[i];
		int AxisValue = AxisValues[i];

		PlayerController->SetKeyAxisBinding(ActionName, AxisValue, InputChord_primary, InputChord_secondary);
	}

	FString ConfigName = PlayerActionToConfigName(PlayerAction, false);
	FString ValueToSave = InputChord_primary.Key.GetFName().ToString()
		+ "," + (InputChord_primary.bShift ? "True" : "False")
		+ "," + (InputChord_primary.bCtrl ? "True" : "False")
		+ "," + (InputChord_primary.bAlt ? "True" : "False")
		+ "," + (InputChord_primary.bCmd ? "True" : "False");
	GetGameInstance<UStalkerMPGameInstance>()->PutStringSettingValue(ESettingsType::Controls, ConfigName, ValueToSave);

	ConfigName = PlayerActionToConfigName(PlayerAction, true);
	ValueToSave = InputChord_secondary.Key.GetFName().ToString()
		+ "," + (InputChord_secondary.bShift ? "True" : "False")
		+ "," + (InputChord_secondary.bCtrl ? "True" : "False")
		+ "," + (InputChord_secondary.bAlt ? "True" : "False")
		+ "," + (InputChord_secondary.bCmd ? "True" : "False");
	GetGameInstance<UStalkerMPGameInstance>()->PutStringSettingValue(ESettingsType::Controls, ConfigName, ValueToSave);

	GetGameInstance<UStalkerMPGameInstance>()->SaveSettings(ESettingsType::Controls);

	OnKeySet(PlayerAction, InputChord_primary, InputChord_secondary);
}

void UControlsSettingsUI::UpdateKeyBinding(EPlayerAction PlayerAction, FInputChord InputChord_primary, FInputChord InputChord_secondary)
{
	if (IsBusySettingKeys)
	{
		return;
	}

	IsBusySettingKeys = true;

	SetKeyBinding(PlayerAction, InputChord_primary, InputChord_secondary);

	IsBusySettingKeys = false;
}
