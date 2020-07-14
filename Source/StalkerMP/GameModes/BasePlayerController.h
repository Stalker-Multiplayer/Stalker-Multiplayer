// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UGameMenuUI;
class USettingsUI;
class UGameUI;
class UMinimapWidget;
class UMapWidget;
class USoundClass;
class USoundMix;
class UMaterialParameterCollection;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
	


// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const int GAME_MENU_ZORDER = 51;
	static const int SETTINGS_ZORDER = 52;

	static const int CUSTOM_GAME_UI_ZORDER = 1;
	static const int CUSTOM_GAME_STARTING_UI_ZORDER = 2;
	static const int BASE_GAME_UI_ZORDER = 3;


public:

	static const int PLAYER_CHARACTER_UI_ZORDER = 0;
	static const int PLAYER_CHARACTER_INVENTORY_ZORDER = 4;

	// Video
	static const FString FULLSCREEN_MODE_SETTING_KEY;
	static const float DEFAULT_FULLSCREEN_MODE;
	static const FString SCREEN_RESOLUTION_SETTING_KEY;
	static const FString DEFAULT_SCREEN_RESOLUTION;
	static const FString SCREEN_PERCENTAGE_SETTING_KEY;
	static const int DEFAULT_SCREEN_PERCENTAGE;
	static const FString BRIGHTNESS_SETTING_KEY;
	static const float DEFAULT_BRIGHTNESS;
	static const FString FIELD_OF_VIEW_SETTING_KEY;
	static const int DEFAULT_FIELD_OF_VIEW;
	static const FString ANTIALIASING_METHOD_SETTING_KEY;
	static const int DEFAULT_ANTIALIASING_METHOD;
	static const FString ANTIALIASING_QUALITY_SETTING_KEY;
	static const int DEFAULT_ANTIALIASING_QUALITY;
	static const FString EFFECTS_QUALITY_SETTING_KEY;
	static const int DEFAULT_EFFECTS_QUALITY;
	static const FString POST_PROCESS_QUALITY_SETTING_KEY;
	static const int DEFAULT_POST_PROCESS_QUALITY;
	static const FString SHADOW_QUALITY_SETTING_KEY;
	static const int DEFAULT_SHADOW_QUALITY;
	static const FString TEXTURE_QUALITY_SETTING_KEY;
	static const int DEFAULT_TEXTURE_QUALITY;
	static const FString AMBIENT_OCCLUSION_SETTING_KEY;
	static const int DEFAULT_AMBIENT_OCCLUSION;
	static const FString BLOOM_SETTING_KEY;
	static const int DEFAULT_BLOOM;
	static const FString PARALLAX_SETTING_KEY;
	static const int DEFAULT_PARALLAX;
	static const FString FOLIAGE_DENSITY_SETTING_KEY;
	static const int DEFAULT_FOLIAGE_DENSITY;
	static const FString FOLIAGE_DISTANCE_SETTING_KEY;
	static const int DEFAULT_FOLIAGE_DISTANCE;
	static const FString SHOW_FPS_SETTING_KEY;
	static const int DEFAULT_SHOW_FPS;

	// Sound
	static const FString MASTER_VOLUME_SETTING_KEY;
	static const float DEFAULT_MASTER_VOLUME;
	static const FString GAME_VOLUME_SETTING_KEY;
	static const float DEFAULT_GAME_VOLUME;
	static const FString MUSIC_VOLUME_SETTING_KEY;
	static const float DEFAULT_MUSIC_VOLUME;

	// HUD
	static const FString CROSSHAIR_LINES_GAP_SETTING_KEY;
	static const float DEFAULT_CROSSHAIR_LINES_GAP;
	static const FString CROSSHAIR_LINES_LENGTH_SETTING_KEY;
	static const float DEFAULT_CROSSHAIR_LINES_LENGTH;
	static const FString CROSSHAIR_LINES_THICKNESS_SETTING_KEY;
	static const float DEFAULT_CROSSHAIR_LINES_THICKNESS;
	static const FString CROSSHAIR_INACCURACY_SCALE_SETTING_KEY;
	static const float DEFAULT_CROSSHAIR_INACCURACY_SCALE;
	static const FString MINIMAP_SIZE_SETTING_KEY;
	static const int DEFAULT_MINIMAP_SIZE;
	static const FString MINIMAP_ZOOM_SETTING_KEY;
	static const float DEFAULT_MINIMAP_ZOOM;
	static const FString MINIMAP_ICONS_SCALE_SETTING_KEY;
	static const float DEFAULT_MINIMAP_ICONS_SIZE;
	static const FString MAP_ICONS_SCALE_SETTING_KEY;
	static const float DEFAULT_MAP_ICONS_SIZE;
	static const FString HIDE_UI_SETTING_KEY;
	static const float DEFAULT_HIDE_UI;

	// Sensitivity
	static const FString HORIZONTAL_SENSITIVITY_SETTING_KEY;
	static const float DEFAULT_HORIZONTAL_SENSITIVITY;
	static const FString VERTICAL_SENSITIVITY_SETTING_KEY;
	static const float DEFAULT_VERTICAL_SENSITIVITY;

	// Movement
	static const FName ACTION_MOVE_FORWARD;
	static const FName ACTION_MOVE_BACKWARD;
	static const FName ACTION_STRAFE_LEFT;
	static const FName ACTION_STRAFE_RIGHT;
	static const FName ACTION_TURN_RIGHT;
	static const FName ACTION_LOOK_UP;
	static const FName ACTION_SPRINT;
	static const FName ACTION_WALK;
	static const FName ACTION_JUMP;
	static const FName ACTION_CROUCH;
	static const FName ACTION_LEAN_LEFT;
	static const FName ACTION_LEAN_RIGHT;

	// Firing
	static const FName ACTION_FIRE_PRIMARY;
	static const FName ACTION_FIRE_SECONDARY;
	static const FName ACTION_RELOAD;
	static const FName ACTION_SWITCH_FIRE_MODE;

	// Inventory
	static const FName ACTION_TOGGLE_INVENTORY;
	static const FName ACTION_PICK_WEAPON_1;
	static const FName ACTION_PICK_WEAPON_2;
	static const FName ACTION_PICK_WEAPON_3;
	static const FName ACTION_USE_BANDAGE;
	static const FName ACTION_USE_FIRST_AID_KIT;

	// Misc
	static const FName ACTION_ACTION;
	static const FName ACTION_TOGGLE_MAP;
	static const FName ACTION_MENU;
	static const FName ACTION_MOUSE_WHEEL_UP;
	static const FName ACTION_MOUSE_WHEEL_DOWN;




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FDelegateHandle OnNetworkFailureDelegateHandle;

	FScriptDelegate OnFloatSettingValueUpdatedDelegate;
	FScriptDelegate OnIntSettingValueUpdatedDelegate;
	FScriptDelegate OnGamemodeLevelLoadedDelegate;
	FScriptDelegate OnPlayerCharacterDiedDelegate;


	UPROPERTY()
		float HorizontalSensitivity = 1.0;

	UPROPERTY()
		float VerticalSensitivity = 1.0;


protected:

	// HUD

	UPROPERTY()
		UGameMenuUI* GameMenuUI;

	UPROPERTY()
		USettingsUI* SettingsUI;

	UPROPERTY()
		UGameUI* BaseGameUI;

	UPROPERTY()
		UUserWidget* CustomGameStartingUI;

	UPROPERTY()
		UUserWidget* CustomGameUI;

	UPROPERTY()
		UUserWidget* CustomGameEndedUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UGameMenuUI> GameMenuUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<USettingsUI> SettingsUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UGameUI> BaseGameUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UUserWidget> CustomGameStartingUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UUserWidget> CustomGameUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UUserWidget> CustomGameEndedUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundMix* MasterSoundMix;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundClass* GameSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UMaterialParameterCollection* MaterialParameterCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName UseParallaxParameterName = FName(TEXT("UseParallax"));


public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UMinimapWidget> MinimapWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UMapWidget> MapWidgetClass;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	// Game

	UFUNCTION()
		virtual void OnGamemodeLevelLoaded();

	UFUNCTION()
		virtual void OnNetworkFailure(ENetworkFailure::Type FailureType, FString ErrorString);

	UFUNCTION()
		void OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(Client, Reliable)
		void Client_OnPlayerDied(const FString &PlayerName, const FString &KillerName, bool KilledByMe, bool MeDied);


	// Settings

	UFUNCTION()
		void OnFloatSettingUpdated(FString Key, float Value);

	UFUNCTION()
		void OnIntSettingUpdated(FString Key, int Value);


	// General

	UFUNCTION()
		virtual void OnMenuButtonPressed();

	UFUNCTION()
		virtual void OnMouseWheelUp();

	UFUNCTION()
		virtual void OnMouseWheelDown();


public:

	// Game

	UFUNCTION()
		virtual void OnMatchStateUpdated(EMatchState NewMatchState);


	// Settings

	UFUNCTION()
		void SetHorizontalSensitivity(float Sensitivity) { HorizontalSensitivity = Sensitivity; }

	UFUNCTION()
		float GetHorizontalSensitivity() { return HorizontalSensitivity; }

	UFUNCTION()
		void SetKeyActionBinding(FName ActionName, FInputChord InputChord_primary, FInputChord InputChord_secondary);

	UFUNCTION()
		void SetKeyAxisBinding(FName ActionName, int AxisValue, FInputChord InputChord_primary, FInputChord InputChord_secondary);

	UFUNCTION()
		void RemoveKeyActionBinding(FName ActionName);

	UFUNCTION()
		void RemoveKeyAxisBinding(FName ActionName);

	UFUNCTION()
		void SetVerticalSensitivity(float Sensitivity) { VerticalSensitivity = Sensitivity; }

	UFUNCTION()
		float GetVerticalSensitivity() { return VerticalSensitivity; }


	// General

	UFUNCTION()
		virtual void UpdateMouseControl();

	UFUNCTION()
		void SetMouseVisible(bool Visible);

	UFUNCTION()
		virtual bool IsAnyMenuVisible();

	UFUNCTION()
		bool IsGameMenuVisible();

	UFUNCTION()
		virtual void HideAllUI(bool HideCustom);

	UFUNCTION()
		void ShowGameMenuUI(bool Show);

	UFUNCTION()
		void ShowSettingsUI(bool Show);

	UFUNCTION()
		void ToggleMapUI();

	UFUNCTION()
		void ShowGameUI(bool Show);

	UFUNCTION()
		void ExitToMainMenu();

	UFUNCTION()
		void QuitToDesktop();

	
};
