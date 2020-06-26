// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalEnums.generated.h"

UENUM(BlueprintType)
enum class EReloadType : uint8
{
	Magazine				UMETA(DisplayName = "Magazine"),
	Single					UMETA(DisplayName = "Single")
};

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	Body						UMETA(DisplayName = "Body"),
	Head						UMETA(DisplayName = "Head"),
	Arm							UMETA(DisplayName = "Arm"),
	Leg							UMETA(DisplayName = "Leg")
};

UENUM(BlueprintType)
enum class ETorsoAnimation : uint8
{
	Torso0						UMETA(DisplayName = "Torso0"),
	Torso1						UMETA(DisplayName = "Torso1"),
	Torso2						UMETA(DisplayName = "Torso2"),
	Torso3						UMETA(DisplayName = "Torso3"),
	Torso4						UMETA(DisplayName = "Torso4"),
	Torso5						UMETA(DisplayName = "Torso5"),
	Torso6						UMETA(DisplayName = "Torso6"),
	Torso7						UMETA(DisplayName = "Torso7"),
	Torso8						UMETA(DisplayName = "Torso8"),
	Torso9						UMETA(DisplayName = "Torso9"),
	Torso10						UMETA(DisplayName = "Torso10"),
	Torso11						UMETA(DisplayName = "Torso11"),
	Torso12						UMETA(DisplayName = "Torso12"),
	Torso13						UMETA(DisplayName = "Torso13")
};

UENUM(BlueprintType)
enum class EWeaponAnimState : uint8
{
	OnGround			UMETA(DisplayName = "OnGround"),
	Hold				UMETA(DisplayName = "Hold"),
	Reloading			UMETA(DisplayName = "Reloading")
};

UENUM(BlueprintType)
enum class EWeaponHoldState : uint8
{
	Ready					UMETA(DisplayName = "Ready"),
	Reloading				UMETA(DisplayName = "Reloading"),
	Showing					UMETA(DisplayName = "Showing"),
	Hiding					UMETA(DisplayName = "Hiding")
};

UENUM()
enum class EMovingMode : uint8
{
	Running				UMETA(DisplayName = "Running"),
	Walking				UMETA(DisplayName = "Walking"),
	Sprinting			UMETA(DisplayName = "Sprinting"),
	SprintingToSide		UMETA(DisplayName = "Sprinting (To Side)"),
	Aimimg				UMETA(DisplayName = "Aiming")
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ST_None		UMETA(DisplayName = "None"),
	ST_Armor	UMETA(DisplayName = "Armor"),
	ST_Weapon1	UMETA(DisplayName = "Weapon 1"),
	ST_Weapon2	UMETA(DisplayName = "Weapon 2")
};

UENUM(BlueprintType)
enum class ESettingsType : uint8
{
	General				UMETA(DisplayName = "General"),
	Video				UMETA(DisplayName = "Video"),
	Sound				UMETA(DisplayName = "Sound"),
	Controls			UMETA(DisplayName = "Controls"),
	Server				UMETA(DisplayName = "Server")
};

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	InLobby						UMETA(DisplayName = "InLobby"),
	Starting					UMETA(DisplayName = "Starting"),
	Ongoing						UMETA(DisplayName = "Ongoing"),
	Ended						UMETA(DisplayName = "Ended"),
	Custom						UMETA(DisplayName = "Custom")
};

UENUM()
enum class EOnSessionJoinResult : uint8
{
	// The join worked as expected
	Success,
	// There are no open slots to join
	SessionIsFull,
	// The session couldn't be found on the service
	SessionDoesNotExist,
	// There was an error getting the session server's address
	CouldNotRetrieveAddress,
	// The user attempting to join is already a member of the session
	AlreadyInSession,
	// An error not covered above occurred
	UnknownError
};

UENUM(BlueprintType)
enum class EPlayerAction : uint8
{
	// Movement
	MoveForward					UMETA(DisplayName = "MoveForward"),
	MoveBackward				UMETA(DisplayName = "MoveBackward"),
	StrafeLeft					UMETA(DisplayName = "StrafeLeft"),
	StrafeRight					UMETA(DisplayName = "StrafeRight"),
	Sprint						UMETA(DisplayName = "Sprint"),
	Walk						UMETA(DisplayName = "Walk"),
	Jump						UMETA(DisplayName = "Jump"),
	Crouch						UMETA(DisplayName = "Crouch"),
	LeanLeft					UMETA(DisplayName = "LeanLeft"),
	LeanRight					UMETA(DisplayName = "LeanRight"),

	// Firing
	FirePrimary					UMETA(DisplayName = "FirePrimary"),
	FireSecondary				UMETA(DisplayName = "FireSecondary"),
	Reload						UMETA(DisplayName = "Reload"),
	SwitchFireMode				UMETA(DisplayName = "SwitchFireMode"),

	// Inventory
	ToggleInventory				UMETA(DisplayName = "ToggleInventory"),
	PickWeapon1					UMETA(DisplayName = "PickWeapon1"),
	PickWeapon2					UMETA(DisplayName = "PickWeapon2"),
	PickWeapon3					UMETA(DisplayName = "PickWeapon3"),
	UseBandage					UMETA(DisplayName = "UseBandage"),
	UseFirstAidKit				UMETA(DisplayName = "UseFirstAidKit"),

	// Misc
	UseAction					UMETA(DisplayName = "UseAction"),
	ToggleMap					UMETA(DisplayName = "ToggleMap"),

	// 
	None						UMETA(DisplayName = "None"),
};

/**
 * 
 */
UCLASS()
class STALKERMP_API UGlobalEnums : public UObject
{
	GENERATED_BODY()
	
};
