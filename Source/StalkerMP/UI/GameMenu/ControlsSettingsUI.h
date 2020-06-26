// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"
#include "StalkerMP/UI/GameMenu/BaseMenuUI.h"

#include "ControlsSettingsUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UControlsSettingsUI : public UBaseMenuUI
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:



	// Movement
	const FString DEFAULT_MOVE_FORWARD = "W,False,False,False,False";
	const FString DEFAULT_MOVE_FORWARD_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_MOVE_BACKWARD = "S,False,False,False,False";
	const FString DEFAULT_MOVE_BACKWARD_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_STRAFE_LEFT = "A,False,False,False,False";
	const FString DEFAULT_STRAFE_LEFT_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_STRAFE_RIGHT = "D,False,False,False,False";
	const FString DEFAULT_STRAFE_RIGHT_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_SPRINT = "LeftShift,False,False,False,False";
	const FString DEFAULT_SPRINT_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_WALK = "LeftControl,False,False,False,False";
	const FString DEFAULT_WALK_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_JUMP = "SpaceBar,False,False,False,False";
	const FString DEFAULT_JUMP_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_CROUCH = "C,False,False,False,False";
	const FString DEFAULT_CROUCH_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_LEAN_LEFT = "Q,False,False,False,False";
	const FString DEFAULT_LEAN_LEFT_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_LEAN_RIGHT = "E,False,False,False,False";
	const FString DEFAULT_LEAN_RIGHT_SECONDARY = "None,False,False,False,False";

	// Firing
	const FString DEFAULT_FIRE_PRIMARY = "LeftMouseButton,False,False,False,False";
	const FString DEFAULT_FIRE_PRIMARY_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_FIRE_SECONDARY = "RightMouseButton,False,False,False,False";
	const FString DEFAULT_FIRE_SECONDARY_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_RELOAD = "R,False,False,False,False";
	const FString DEFAULT_RELOAD_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_SWITCH_FIRE_MODE = "B,False,False,False,False";
	const FString DEFAULT_SWITCH_FIRE_MODE_SECONDARY = "None,False,False,False,False";

	// Inventory
	const FString DEFAULT_TOGGLE_INVENTORY = "Tab,False,False,False,False";
	const FString DEFAULT_TOGGLE_INVENTORY_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_PICK_WEAPON_1 = "One,False,False,False,False";
	const FString DEFAULT_PICK_WEAPON_1_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_PICK_WEAPON_2 = "Two,False,False,False,False";
	const FString DEFAULT_PICK_WEAPON_2_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_PICK_WEAPON_3 = "Three,False,False,False,False";
	const FString DEFAULT_PICK_WEAPON_3_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_USE_BANDAGE = "Four,False,False,False,False";
	const FString DEFAULT_USE_BANDAGE_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_USE_FIRST_AID_KIT = "Five,False,False,False,False";
	const FString DEFAULT_USE_FIRST_AID_KIT_SECONDARY = "None,False,False,False,False";

	// Misc
	const FString DEFAULT_ACTION = "F,False,False,False,False";
	const FString DEFAULT_ACTION_SECONDARY = "None,False,False,False,False";
	const FString DEFAULT_TOGGLE_MAP = "M,False,False,False,False";
	const FString DEFAULT_TOGGLE_MAP_SECONDARY = "None,False,False,False,False";


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	void NativeConstruct() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		bool IsBusySettingKeys = false;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:


	// Key bindings

	UFUNCTION()
		FString PlayerActionToConfigName(EPlayerAction PlayerAction, bool IsSecondary);

	UFUNCTION()
		FString DefaultPlayerActionKey(EPlayerAction PlayerAction, bool IsSecondary);

	UFUNCTION()
		void PlayerActionToActionNames(EPlayerAction PlayerAction, TArray<FName> &ActionNames);

	UFUNCTION()
		void PlayerActionToAxisNames(EPlayerAction PlayerAction, TArray<FName> &AxisNames, TArray<int> &AxisValues);

	UFUNCTION()
		void ReadAndApplyKeySetting(EPlayerAction PlayerAction);

	UFUNCTION()
		void SetKeyBinding(EPlayerAction PlayerAction, FInputChord InputChord_primary, FInputChord InputChord_secondary);


protected:

	// Sensitivity

	UFUNCTION(BlueprintCallable)
		void SetHorizontalSensitivity(float Sensitivity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnHorizontalSensitivityUpdated(float NewSensitivity);

	UFUNCTION(BlueprintCallable)
		void SetVerticalSensitivity(float Sensitivity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnVerticalSensitivityUpdated(float NewSensitivity);


	// Key bindings

	UFUNCTION(BlueprintCallable)
		void UpdateKeyBinding(EPlayerAction PlayerAction, FInputChord InputChord_primary, FInputChord InputChord_secondary);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnKeySet(EPlayerAction PlayerAction, FInputChord InputChord_primary, FInputChord InputChord_secondary);



};
