// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerController.h"

#include "StalkerMP/GameModes/BaseGameState.h"
#include "StalkerMP/DataClasses/BaseLevelDataActor.h"
#include "StalkerMP/UI/GameMenu/GameMenuUI.h"
#include "StalkerMP/UI/GameMenu/SettingsUI.h"
#include "StalkerMP/UI/GameUI.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"


// Video
const FString ABasePlayerController::FULLSCREEN_MODE_SETTING_KEY = "FullscreenMode";
const float ABasePlayerController::DEFAULT_FULLSCREEN_MODE = 1;
const FString ABasePlayerController::SCREEN_RESOLUTION_SETTING_KEY = "ScreenResolution";
const FString ABasePlayerController::DEFAULT_SCREEN_RESOLUTION = "NULL";
const FString ABasePlayerController::SCREEN_PERCENTAGE_SETTING_KEY = "ScreenPercentage";
const int ABasePlayerController::DEFAULT_SCREEN_PERCENTAGE = 100;
const FString ABasePlayerController::BRIGHTNESS_SETTING_KEY = "Brightness";
const float ABasePlayerController::DEFAULT_BRIGHTNESS = 2.2;
const FString ABasePlayerController::FIELD_OF_VIEW_SETTING_KEY = "FieldOfView";
const int ABasePlayerController::DEFAULT_FIELD_OF_VIEW = 90;
const FString ABasePlayerController::ANTIALIASING_METHOD_SETTING_KEY = "AntialiasingMethod";
const int ABasePlayerController::DEFAULT_ANTIALIASING_METHOD = 1;
const FString ABasePlayerController::ANTIALIASING_QUALITY_SETTING_KEY = "AntialiasingQuality";
const int ABasePlayerController::DEFAULT_ANTIALIASING_QUALITY = 2;
const FString ABasePlayerController::EFFECTS_QUALITY_SETTING_KEY = "EffectsQuality";
const int ABasePlayerController::DEFAULT_EFFECTS_QUALITY = 2;
const FString ABasePlayerController::POST_PROCESS_QUALITY_SETTING_KEY = "PostProcessQuality";
const int ABasePlayerController::DEFAULT_POST_PROCESS_QUALITY = 2;
const FString ABasePlayerController::SHADOW_QUALITY_SETTING_KEY = "ShadowQuality";
const int ABasePlayerController::DEFAULT_SHADOW_QUALITY = 2;
const FString ABasePlayerController::TEXTURE_QUALITY_SETTING_KEY = "TextureQuality";
const int ABasePlayerController::DEFAULT_TEXTURE_QUALITY = 2;
const FString ABasePlayerController::AMBIENT_OCCLUSION_SETTING_KEY = "AmbientOcclusion";
const int ABasePlayerController::DEFAULT_AMBIENT_OCCLUSION = 1;
const FString ABasePlayerController::BLOOM_SETTING_KEY = "Bloom";
const int ABasePlayerController::DEFAULT_BLOOM = 1;
const FString ABasePlayerController::PARALLAX_SETTING_KEY = "Parallax";
const int ABasePlayerController::DEFAULT_PARALLAX = 0;
const FString ABasePlayerController::FOLIAGE_DENSITY_SETTING_KEY = "FoliageDensity";
const int ABasePlayerController::DEFAULT_FOLIAGE_DENSITY = 4;
const FString ABasePlayerController::FOLIAGE_DISTANCE_SETTING_KEY = "FoliageDistance";
const int ABasePlayerController::DEFAULT_FOLIAGE_DISTANCE = 2;
const FString ABasePlayerController::SHOW_FPS_SETTING_KEY = "ShowFPS";
const int ABasePlayerController::DEFAULT_SHOW_FPS = 0;

// Sound
const FString ABasePlayerController::MASTER_VOLUME_SETTING_KEY = "MasterVolume";
const float ABasePlayerController::DEFAULT_MASTER_VOLUME = 1;
const FString ABasePlayerController::GAME_VOLUME_SETTING_KEY = "GameVolume";
const float ABasePlayerController::DEFAULT_GAME_VOLUME = 1;
const FString ABasePlayerController::MUSIC_VOLUME_SETTING_KEY = "MusicVolume";
const float ABasePlayerController::DEFAULT_MUSIC_VOLUME = 1;

// HUD
const FString ABasePlayerController::CROSSHAIR_LINES_GAP_SETTING_KEY = "CrosshairLinesGap";
const float ABasePlayerController::DEFAULT_CROSSHAIR_LINES_GAP = 0;
const FString ABasePlayerController::CROSSHAIR_LINES_LENGTH_SETTING_KEY = "CrosshairLinesLength";
const float ABasePlayerController::DEFAULT_CROSSHAIR_LINES_LENGTH = 10;
const FString ABasePlayerController::CROSSHAIR_LINES_THICKNESS_SETTING_KEY = "CrosshairLinesThickness";
const float ABasePlayerController::DEFAULT_CROSSHAIR_LINES_THICKNESS = 2;
const FString ABasePlayerController::CROSSHAIR_INACCURACY_SCALE_SETTING_KEY = "CrosshairInaccuracyScale";
const float ABasePlayerController::DEFAULT_CROSSHAIR_INACCURACY_SCALE = 1;
const FString ABasePlayerController::MINIMAP_SIZE_SETTING_KEY = "MinimapSize";
const int ABasePlayerController::DEFAULT_MINIMAP_SIZE = 250;
const FString ABasePlayerController::MINIMAP_ZOOM_SETTING_KEY = "MinimapZoom";
const float ABasePlayerController::DEFAULT_MINIMAP_ZOOM = 8;
const FString ABasePlayerController::MINIMAP_ICONS_SCALE_SETTING_KEY = "MinimapIconsScale";
const float ABasePlayerController::DEFAULT_MINIMAP_ICONS_SIZE = 1;
const FString ABasePlayerController::MAP_ICONS_SCALE_SETTING_KEY = "MapIconsScale";
const float ABasePlayerController::DEFAULT_MAP_ICONS_SIZE = 1;
const FString ABasePlayerController::HIDE_UI_SETTING_KEY = "HideUI";
const float ABasePlayerController::DEFAULT_HIDE_UI = 0;

// Sensitivity
const FString ABasePlayerController::HORIZONTAL_SENSITIVITY_SETTING_KEY = "HorizontalSensitivity";
const float ABasePlayerController::DEFAULT_HORIZONTAL_SENSITIVITY = 1;
const FString ABasePlayerController::VERTICAL_SENSITIVITY_SETTING_KEY = "VerticalSensitivity";
const float ABasePlayerController::DEFAULT_VERTICAL_SENSITIVITY = 1;

// Movement
const FName ABasePlayerController::ACTION_MOVE_FORWARD = "MoveForward";
const FName ABasePlayerController::ACTION_MOVE_BACKWARD = "MoveBackward";
const FName ABasePlayerController::ACTION_STRAFE_LEFT = "StrafeLeft";
const FName ABasePlayerController::ACTION_STRAFE_RIGHT = "StrafeRight";
const FName ABasePlayerController::ACTION_TURN_RIGHT = "TurnRight";
const FName ABasePlayerController::ACTION_LOOK_UP = "LookUp";
const FName ABasePlayerController::ACTION_SPRINT = "Sprint";
const FName ABasePlayerController::ACTION_WALK = "Walk";
const FName ABasePlayerController::ACTION_JUMP = "Jump";
const FName ABasePlayerController::ACTION_CROUCH = "Crouch";

// Firing
const FName ABasePlayerController::ACTION_FIRE_PRIMARY = "FirePrimary";
const FName ABasePlayerController::ACTION_FIRE_SECONDARY = "FireSecondary";
const FName ABasePlayerController::ACTION_RELOAD = "Reload";
const FName ABasePlayerController::ACTION_SWITCH_FIRE_MODE = "SwitchFireMode";

// Inventory
const FName ABasePlayerController::ACTION_TOGGLE_INVENTORY = "ToggleInventory";
const FName ABasePlayerController::ACTION_PICK_WEAPON_1 = "PickWeapon1";
const FName ABasePlayerController::ACTION_PICK_WEAPON_2 = "PickWeapon2";
const FName ABasePlayerController::ACTION_PICK_WEAPON_3 = "PickWeapon3";
const FName ABasePlayerController::ACTION_USE_BANDAGE = "UseBandage";
const FName ABasePlayerController::ACTION_USE_FIRST_AID_KIT = "UseFirstAidKit";

// Misc
const FName ABasePlayerController::ACTION_ACTION = "Action";
const FName ABasePlayerController::ACTION_TOGGLE_MAP = "ToggleMap";
const FName ABasePlayerController::ACTION_MENU = "Menu";
const FName ABasePlayerController::ACTION_MOUSE_WHEEL_UP = "MouseWheelUp";
const FName ABasePlayerController::ACTION_MOUSE_WHEEL_DOWN = "MouseWheelDown";


void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	if (IsLocalController())
	{
		UStalkerMPGameInstance::FOnNetworkFailureDelegate OnNetworkFailureDelegate = UStalkerMPGameInstance::FOnNetworkFailureDelegate::CreateUObject(this, &ABasePlayerController::OnNetworkFailure);
		OnNetworkFailureDelegateHandle = GetGameInstance<UStalkerMPGameInstance>()->AddOnNetworkFailureDelegate_Handle(OnNetworkFailureDelegate);

		ConsoleCommand("t.MaxFPS 0");
		ConsoleCommand("ShowFlag.ScreenSpaceReflections 0");
		
		OnFloatSettingUpdated(HORIZONTAL_SENSITIVITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetFloatSettingValue(ESettingsType::Controls, HORIZONTAL_SENSITIVITY_SETTING_KEY, DEFAULT_MASTER_VOLUME));
		OnFloatSettingUpdated(VERTICAL_SENSITIVITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetFloatSettingValue(ESettingsType::Controls, VERTICAL_SENSITIVITY_SETTING_KEY, DEFAULT_MASTER_VOLUME));

		OnFloatSettingUpdated(MASTER_VOLUME_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetFloatSettingValue(ESettingsType::Sound, MASTER_VOLUME_SETTING_KEY, DEFAULT_MASTER_VOLUME));
		OnFloatSettingUpdated(GAME_VOLUME_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetFloatSettingValue(ESettingsType::Sound, GAME_VOLUME_SETTING_KEY, DEFAULT_GAME_VOLUME));
		OnFloatSettingUpdated(MUSIC_VOLUME_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetFloatSettingValue(ESettingsType::Sound, MUSIC_VOLUME_SETTING_KEY, DEFAULT_MUSIC_VOLUME));

		OnIntSettingUpdated(SCREEN_PERCENTAGE_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, SCREEN_PERCENTAGE_SETTING_KEY, DEFAULT_SCREEN_PERCENTAGE));
		OnFloatSettingUpdated(BRIGHTNESS_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetFloatSettingValue(ESettingsType::Video, BRIGHTNESS_SETTING_KEY, DEFAULT_BRIGHTNESS));
		OnIntSettingUpdated(ANTIALIASING_METHOD_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, ANTIALIASING_METHOD_SETTING_KEY, DEFAULT_ANTIALIASING_METHOD));
		OnIntSettingUpdated(ANTIALIASING_QUALITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, ANTIALIASING_QUALITY_SETTING_KEY, DEFAULT_ANTIALIASING_QUALITY));
		OnIntSettingUpdated(EFFECTS_QUALITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, EFFECTS_QUALITY_SETTING_KEY, DEFAULT_EFFECTS_QUALITY));
		OnIntSettingUpdated(POST_PROCESS_QUALITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, POST_PROCESS_QUALITY_SETTING_KEY, DEFAULT_POST_PROCESS_QUALITY));
		OnIntSettingUpdated(SHADOW_QUALITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, SHADOW_QUALITY_SETTING_KEY, DEFAULT_SHADOW_QUALITY));
		OnIntSettingUpdated(TEXTURE_QUALITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, TEXTURE_QUALITY_SETTING_KEY, DEFAULT_TEXTURE_QUALITY));
		OnIntSettingUpdated(AMBIENT_OCCLUSION_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, AMBIENT_OCCLUSION_SETTING_KEY, DEFAULT_AMBIENT_OCCLUSION));
		OnIntSettingUpdated(BLOOM_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, BLOOM_SETTING_KEY, DEFAULT_BLOOM));
		OnIntSettingUpdated(PARALLAX_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, PARALLAX_SETTING_KEY, DEFAULT_PARALLAX));
		OnIntSettingUpdated(FOLIAGE_DENSITY_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, FOLIAGE_DENSITY_SETTING_KEY, DEFAULT_FOLIAGE_DENSITY));
		OnIntSettingUpdated(FOLIAGE_DISTANCE_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, FOLIAGE_DISTANCE_SETTING_KEY, DEFAULT_FOLIAGE_DISTANCE));
		OnIntSettingUpdated(SHOW_FPS_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
			->GetIntSettingValue(ESettingsType::Video, SHOW_FPS_SETTING_KEY, DEFAULT_SHOW_FPS));

		OnFloatSettingValueUpdatedDelegate.BindUFunction(this, "OnFloatSettingUpdated");
		UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.AddUnique(OnFloatSettingValueUpdatedDelegate);
		OnIntSettingValueUpdatedDelegate.BindUFunction(this, "OnIntSettingUpdated");
		UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.AddUnique(OnIntSettingValueUpdatedDelegate);

		GameMenuUI = CreateWidget<UGameMenuUI>(this, GameMenuUIClass);
		GameMenuUI->SetPlayerController(this);
		GameMenuUI->AddToViewport(GAME_MENU_ZORDER);
		GameMenuUI->SetVisibility(ESlateVisibility::Collapsed);

		SettingsUI = CreateWidget<USettingsUI>(this, SettingsUIClass);
		SettingsUI->SetPlayerController(this);
		SettingsUI->AddToViewport(SETTINGS_ZORDER);
		SettingsUI->SetVisibility(ESlateVisibility::Collapsed);

		if (CustomGameStartingUIClass.Get())
		{
			CustomGameStartingUI = CreateWidget<UUserWidget>(this, CustomGameStartingUIClass);
			CustomGameStartingUI->AddToViewport(CUSTOM_GAME_STARTING_UI_ZORDER);
			CustomGameStartingUI->SetVisibility(ESlateVisibility::Visible);
		}

		if (CustomGameUIClass.Get())
		{
			CustomGameUI = CreateWidget<UUserWidget>(this, CustomGameUIClass);
			CustomGameUI->AddToViewport(CUSTOM_GAME_UI_ZORDER);
			CustomGameUI->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (CustomGameEndedUIClass.Get())
		{
			CustomGameEndedUI = CreateWidget<UUserWidget>(this, CustomGameEndedUIClass);
			CustomGameEndedUI->AddToViewport(CUSTOM_GAME_UI_ZORDER);
			CustomGameEndedUI->SetVisibility(ESlateVisibility::Collapsed);
		}

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseLevelDataActor::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			OnGamemodeLevelLoaded();
		}
		else
		{
			OnGamemodeLevelLoadedDelegate.BindUFunction(this, "OnGamemodeLevelLoaded");
			ABaseLevelDataActor::GamemodeLevelLoadedDelegate.AddUnique(OnGamemodeLevelLoadedDelegate);
		}

		GetWorld()->GetGameState<ABaseGameState>()->AddLocalPlayer(this);
	}
	else if (HasAuthority())
	{
		OnPlayerCharacterDiedDelegate.BindUFunction(this, "OnPlayerCharacterDied");
		APlayerCharacter::OnPlayerCharacterDiedDelegate.AddUnique(OnPlayerCharacterDiedDelegate);
	}
}

void ABasePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetGameInstance<UStalkerMPGameInstance>()->DisconnectFromServer();

	if (IsLocalController())
	{
		GetGameInstance<UStalkerMPGameInstance>()->ClearOnNetworkFailureDelegate_Handle(OnNetworkFailureDelegateHandle);

		UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.Remove(OnFloatSettingValueUpdatedDelegate);
		UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.Remove(OnIntSettingValueUpdatedDelegate);
		ABaseLevelDataActor::GamemodeLevelLoadedDelegate.Remove(OnGamemodeLevelLoadedDelegate);
	}
	else if (HasAuthority())
	{
		APlayerCharacter::OnPlayerCharacterDiedDelegate.Remove(OnPlayerCharacterDiedDelegate);
	}

	if (GameMenuUI)
	{
		GameMenuUI->RemoveFromParent();
		GameMenuUI = nullptr;
	}

	if (SettingsUI)
	{
		SettingsUI->RemoveFromParent();
		SettingsUI = nullptr;
	}

	if (CustomGameUI)
	{
		CustomGameUI->RemoveFromParent();
		CustomGameUI = nullptr;
	}

	if (CustomGameStartingUI)
	{
		CustomGameStartingUI->RemoveFromParent();
		CustomGameStartingUI = nullptr;
	}
	if (CustomGameUI)
	{
		CustomGameUI->RemoveFromParent();
		CustomGameUI = nullptr;
	}
	if (CustomGameEndedUI)
	{
		CustomGameEndedUI->RemoveFromParent();
		CustomGameEndedUI = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Called to bind functionality to input
void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalController())
	{
		InputComponent->BindAction(ABasePlayerController::ACTION_MENU, IE_Pressed, this, &ABasePlayerController::OnMenuButtonPressed);
		InputComponent->BindAction(ABasePlayerController::ACTION_MOUSE_WHEEL_UP, IE_Pressed, this, &ABasePlayerController::OnMouseWheelUp);
		InputComponent->BindAction(ABasePlayerController::ACTION_MOUSE_WHEEL_DOWN, IE_Pressed, this, &ABasePlayerController::OnMouseWheelDown);
		InputComponent->BindAction(ABasePlayerController::ACTION_TOGGLE_MAP, IE_Pressed, this, &ABasePlayerController::ToggleMapUI);
	}
}

void ABasePlayerController::OnGamemodeLevelLoaded()
{
	ABaseLevelDataActor::GamemodeLevelLoadedDelegate.Remove(OnGamemodeLevelLoadedDelegate);

	if (BaseGameUIClass.Get())
	{
		BaseGameUI = CreateWidget<UGameUI>(this, BaseGameUIClass);
		BaseGameUI->AddToViewport(BASE_GAME_UI_ZORDER);
		BaseGameUI->SetPlayerController(this);
	}
}

void ABasePlayerController::OnNetworkFailure(ENetworkFailure::Type FailureType, FString ErrorString)
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void ABasePlayerController::OnMatchStateUpdated(EMatchState NewMatchState)
{
	if (CustomGameUI)
	{
		CustomGameUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CustomGameStartingUI)
	{
		CustomGameStartingUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (CustomGameEndedUI)
	{
		CustomGameEndedUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	switch (NewMatchState)
	{
	case EMatchState::InLobby:
		break;
	case EMatchState::Starting:
		if (CustomGameStartingUI)
		{
			CustomGameStartingUI->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	case EMatchState::Ongoing:
		if (CustomGameUI)
		{
			CustomGameUI->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	case EMatchState::Ended:
		if (CustomGameEndedUI)
		{
			CustomGameEndedUI->SetVisibility(ESlateVisibility::Visible);
		}
		break;
	}
}

void ABasePlayerController::OnPlayerCharacterDied(APlayerCharacter* PlayerCharacter, AController* Controller, AController* InstigatedBy, AActor* DamageCauser)
{
	FString DiedPlayerName = PlayerCharacter->GetPlayerName();
	FString KillerName;
	if (InstigatedBy)
	{
		KillerName = InstigatedBy->PlayerState->GetPlayerName();
	}
	else
	{
		KillerName = "";
	}

	Client_OnPlayerDied(DiedPlayerName, KillerName, InstigatedBy == this, Controller == this);
}

void ABasePlayerController::Client_OnPlayerDied_Implementation(const FString &PlayerName, const FString &KillerName, bool KilledByMe, bool MeDied)
{
	if (BaseGameUI)
	{
		if (KilledByMe || MeDied)
		{
			BaseGameUI->OnPlayerDied(PlayerName, KillerName, KilledByMe, MeDied);
		}
	}
}

void ABasePlayerController::OnFloatSettingUpdated(FString Key, float Value)
{
	if (Key.Equals(MASTER_VOLUME_SETTING_KEY))
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MasterSoundClass, Value, 1, 0, true);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
	}
	else if (Key.Equals(GAME_VOLUME_SETTING_KEY))
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, GameSoundClass, Value, 1, 0, true);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
	}
	else if (Key.Equals(MUSIC_VOLUME_SETTING_KEY))
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MusicSoundClass, Value, 1, 0, true);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
	}
	else if (Key.Equals(HORIZONTAL_SENSITIVITY_SETTING_KEY))
	{
		SetHorizontalSensitivity(Value);
	}
	else if (Key.Equals(VERTICAL_SENSITIVITY_SETTING_KEY))
	{
		SetVerticalSensitivity(Value);
	}
	else if (Key.Equals(BRIGHTNESS_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("r.gamma %f"), Value);
		ConsoleCommand(*Command);
	}
}

void ABasePlayerController::OnIntSettingUpdated(FString Key, int Value)
{
	if (Key.Equals(SCREEN_PERCENTAGE_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("r.ScreenPercentage %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(ANTIALIASING_METHOD_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("r.DefaultFeature.AntiAliasing %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(ANTIALIASING_QUALITY_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.AntiAliasingQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(EFFECTS_QUALITY_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.EffectsQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(POST_PROCESS_QUALITY_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.PostProcessQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(SHADOW_QUALITY_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.ShadowQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(TEXTURE_QUALITY_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.TextureQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(AMBIENT_OCCLUSION_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("ShowFlag.ScreenSpaceAO %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(BLOOM_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("ShowFlag.Bloom %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(PARALLAX_SETTING_KEY))
	{
		UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, UseParallaxParameterName, Value);
	}
	else if (Key.Equals(FOLIAGE_DENSITY_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.FoliageQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(FOLIAGE_DISTANCE_SETTING_KEY))
	{
		FString Command = FString::Printf(TEXT("sg.ViewDistanceQuality %d"), Value);
		ConsoleCommand(*Command);
	}
	else if (Key.Equals(SHOW_FPS_SETTING_KEY))
	{
		bool FPSShown = GEngine->GameViewport->IsStatEnabled("FPS");
		if ((Value <= 0 && FPSShown) || (Value > 0 && !FPSShown))
		{
			FString Command = FString::Printf(TEXT("stat FPS"), Value);
			ConsoleCommand(*Command);
		}
	}
}

void ABasePlayerController::SetKeyActionBinding(FName ActionName, FInputChord InputChord_primary, FInputChord InputChord_secondary)
{
	RemoveKeyActionBinding(ActionName);

	if (InputChord_primary.Key == InputChord_secondary.Key)
	{
		InputChord_secondary = FInputChord();
	}

	const UInputSettings* InputSettings = GetDefault<UInputSettings>();

	if (InputChord_primary.Key != EKeys::Invalid)
	{
		FInputActionKeyMapping InputActionKeyMapping = FInputActionKeyMapping(ActionName, InputChord_primary.Key, InputChord_primary.bShift, InputChord_primary.bCtrl, InputChord_primary.bAlt, InputChord_primary.bCmd);
		((UInputSettings*)InputSettings)->AddActionMapping(InputActionKeyMapping, true);
	}

	if (InputChord_secondary.Key != EKeys::Invalid)
	{
		FInputActionKeyMapping InputActionKeyMapping = FInputActionKeyMapping(ActionName, InputChord_secondary.Key, InputChord_secondary.bShift, InputChord_secondary.bCtrl, InputChord_secondary.bAlt, InputChord_secondary.bCmd);
		((UInputSettings*)InputSettings)->AddActionMapping(InputActionKeyMapping, true);
	}

	((UInputSettings*)InputSettings)->SaveKeyMappings();
}

void ABasePlayerController::SetKeyAxisBinding(FName ActionName, int AxisValue, FInputChord InputChord_primary, FInputChord InputChord_secondary)
{
	RemoveKeyAxisBinding(ActionName);

	if (InputChord_primary.Key == InputChord_secondary.Key)
	{
		InputChord_secondary = FInputChord();
	}

	const UInputSettings* InputSettings = GetDefault<UInputSettings>();

	if (InputChord_primary.Key != EKeys::Invalid)
	{
		FInputAxisKeyMapping InputAxisKeyMapping = FInputAxisKeyMapping(ActionName, InputChord_primary.Key, AxisValue);
		((UInputSettings*)InputSettings)->AddAxisMapping(InputAxisKeyMapping, true);
	}

	if (InputChord_secondary.Key != EKeys::Invalid)
	{
		FInputAxisKeyMapping InputAxisKeyMapping = FInputAxisKeyMapping(ActionName, InputChord_secondary.Key, AxisValue);
		((UInputSettings*)InputSettings)->AddAxisMapping(InputAxisKeyMapping, true);
	}

	((UInputSettings*)InputSettings)->SaveKeyMappings();
}

void ABasePlayerController::RemoveKeyActionBinding(FName ActionName)
{
	const UInputSettings* InputSettings = GetDefault<UInputSettings>();

	// Saving as an action
	TArray<FInputActionKeyMapping> AllAssignedActions;
	((UInputSettings*)InputSettings)->GetActionMappingByName(ActionName, AllAssignedActions);

	for (FInputActionKeyMapping AssignedAction : AllAssignedActions)
	{
		((UInputSettings*)InputSettings)->RemoveActionMapping(AssignedAction, true);
	}

	((UInputSettings*)InputSettings)->SaveKeyMappings();
}

void ABasePlayerController::RemoveKeyAxisBinding(FName ActionName)
{
	const UInputSettings* InputSettings = GetDefault<UInputSettings>();

	// Saving as an axis
	TArray<FInputAxisKeyMapping> AllAssignedAxis;
	((UInputSettings*)InputSettings)->GetAxisMappingByName(ActionName, AllAssignedAxis);

	for (FInputAxisKeyMapping AssignedAction : AllAssignedAxis)
	{
		((UInputSettings*)InputSettings)->RemoveAxisMapping(AssignedAction, true);
	}

	((UInputSettings*)InputSettings)->SaveKeyMappings();
}

void ABasePlayerController::OnMenuButtonPressed()
{
	if (IsAnyMenuVisible())
	{
		HideAllUI(true);
	}
	else
	{
		ShowGameMenuUI(true);
	}
}

void ABasePlayerController::OnMouseWheelUp()
{
	if (BaseGameUI && BaseGameUI->IsMapVisible())
	{
		BaseGameUI->MapZoomIn();
	}
}

void ABasePlayerController::OnMouseWheelDown()
{
	if (BaseGameUI && BaseGameUI->IsMapVisible())
	{
		BaseGameUI->MapZoomOut();
	}
}

bool ABasePlayerController::IsAnyMenuVisible()
{
	bool CustomUIVisible = false;
	if (APlayerCharacter* PlayerCharacter = dynamic_cast<APlayerCharacter*>(GetPawn())) {
		CustomUIVisible = PlayerCharacter->IsCustomUIVisible();
	}

	return IsGameMenuVisible()
		|| (BaseGameUI && BaseGameUI->IsMapVisible())
		|| CustomUIVisible;
}

bool ABasePlayerController::IsGameMenuVisible()
{
	return (GameMenuUI && GameMenuUI->GetVisibility() == ESlateVisibility::Visible)
		|| (SettingsUI && SettingsUI->GetVisibility() == ESlateVisibility::Visible);
}

void ABasePlayerController::HideAllUI(bool HideCustom)
{
	ShowSettingsUI(false);
	ShowGameMenuUI(false);
	ShowGameUI(false);

	if (HideCustom) {
		if (APlayerCharacter* PlayerCharacter = dynamic_cast<APlayerCharacter*>(GetPawn())) {
			PlayerCharacter->HideCustomUI();
		}
	}
}

void ABasePlayerController::UpdateMouseControl()
{
	if (IsAnyMenuVisible())
	{
		SetMouseVisible(true);
	}
	else
	{
		SetMouseVisible(false);
	}
}

void ABasePlayerController::SetMouseVisible(bool Visible)
{
	if (bShowMouseCursor != Visible)
	{
		if (Visible)
		{
			SetInputMode(FInputModeGameAndUI());
			bShowMouseCursor = true;

			FViewport* Vieport = CastChecked<ULocalPlayer>(this->Player)->ViewportClient->Viewport;
			Vieport->SetMouse(Vieport->GetSizeXY().X / 2, Vieport->GetSizeXY().Y / 2);
		}
		else
		{
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}
}

void ABasePlayerController::ShowGameMenuUI(bool Show)
{
	if (Show)
	{
		GameMenuUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GameMenuUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	UpdateMouseControl();
}

void ABasePlayerController::ShowSettingsUI(bool Show)
{
	if (Show)
	{
		SettingsUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SettingsUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	UpdateMouseControl();
}

void ABasePlayerController::ToggleMapUI()
{
	if (BaseGameUI)
	{
		if (!BaseGameUI->IsMapVisible() && !IsGameMenuVisible())
		{
			ShowGameUI(true);
		}
		else
		{
			ShowGameUI(false);
		}
	}
}

void ABasePlayerController::ShowGameUI(bool Show)
{
	if (BaseGameUI)
	{
		if (Show)
		{
			if (APlayerCharacter* PlayerCharacter = dynamic_cast<APlayerCharacter*>(GetPawn())) {
				PlayerCharacter->HideCustomUI();
			}
			BaseGameUI->ShowMap();
		}
		else
		{
			BaseGameUI->HideMap();
		}
		UpdateMouseControl();
	}
}

void ABasePlayerController::ExitToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void ABasePlayerController::QuitToDesktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}


