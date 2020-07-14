// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ABaseWeaponItem;
class AFirearmWeaponItem;
class ADeadBodyBackpack;
class UInventoryComponent;
class UPlayerCharacterUI;
class UInventoryUI;

class UAudioComponent;
class UCameraComponent;
class USphereComponent;

DECLARE_DELEGATE(FActionDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnPlayerCharacterDiedDelegate, APlayerCharacter*, PlayerCharacter, AController*, Controller, AController*, InstigatedBy, AActor*, DamageCauser, ADeadBodyBackpack*, DeadBodyBackpack);

UCLASS()
class STALKERMP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

protected:

	static const int MAX_DAMAGE_FOR_MAX_PAIN = 50;
	static const int PAIN_DECREASE_SPEED = 1;




// ------------------------------------------------------------------------- Delegates --------------------------------------------------------------------------

public:

	static FOnPlayerCharacterDiedDelegate OnPlayerCharacterDiedDelegate;




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;

	virtual void Landed(const FHitResult& Hit) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:


	FDelegateHandle OnIntSettingValueUpdatedDelegateHandle;

	// Movement

	UPROPERTY()
		bool bAimActionPressed = false;

	UPROPERTY()
		bool bWalkActionPressed = false;

	UPROPERTY()
		bool bSprintActionPressed = false;

	UPROPERTY()
		bool bCrouchActionPressed = false;

	UPROPERTY()
		bool bMoveForwardPressed = false;

	UPROPERTY()
		float JustLandedSpeedScale = 1.0;


	// Transformation

	UPROPERTY()
		bool bLeanLeftPressed = false;

	UPROPERTY()
		bool bLeanRightPressed = false;

	UPROPERTY()
		float LeanAngle = 0;

	UPROPERTY(Replicated)
		float TargetLeanAngle = 0;

	UPROPERTY()
		FTransform BaseFirstPersonMeshTransform;


	// Items

	UPROPERTY()
		FTimerHandle ItemLocationUpdateTimerHandle;


	// Etc

	UPROPERTY()
		FTimerHandle LongActionTimerHandle;

	UPROPERTY()
		bool DoingLongAction;

	UPROPERTY()
		float PainStrength = 0.0F;


protected:


	// HUD
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UPlayerCharacterUI> PlayerCharacterUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
		UPlayerCharacterUI* PlayerCharacterUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		TSubclassOf<UInventoryUI> InventoryUIClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
		UInventoryUI* InventoryUI;


	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* PlayerCamera;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* ActionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadOnly)
		ADeadBodyBackpack* DeadBodyBackpack;

	UPROPERTY(BlueprintReadOnly)
		ADeadBodyBackpack* OpenDeadBodyBackpack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ADeadBodyBackpack> DeadBodyBackpackClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName WeaponSocketName = "weapon_socket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName CameraSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName MounthSocketName;


	// Movement

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Movement")
		EMovingMode MovingMode = EMovingMode::Running;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float AimimgSpeed = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float WalkingSpeed = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float RunningSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float SprintingSpeed = 750;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float CancelLongActionSpeedThreshold = 350;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float CrouchAimimgSpeed = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float CrouchWalkingSpeed = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float CrouchRunningSpeed = 350;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float CrouchSprintingSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float CrouchCancelLongActionSpeedThreshold = 250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		int MinVerticalVelocityToGetFallDamage = 1250;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float MinJustLandedSpeedScale = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float SecondsToRestoreFromFullFallStop = 2;


	// Status

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
		int MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Status")
		int Health = MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsDead, Category = "Status")
		bool IsDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
		TArray<USoundBase*> DeathSounds;


	// Transformation

	UPROPERTY(BlueprintReadOnly, Category = "Transformation")
		FRotator LookRotator = FRotator();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Transformation")
		float MaxLeanAngle = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Transformation")
		float LeaningSpeed = 150;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Transformation")
		float AimFieldOfViewScale = 1.0f;


	// Items

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ABaseStackableItem> BandageItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ABaseStackableItem> FirstAidKitItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
		USkeletalMesh* DefaultCharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
		USkeletalMesh* DefaultHandsMesh;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorItem, Category = "Items")
		AArmorItem* ArmorItem;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WeaponItem, Category = "Items")
		ABaseWeaponItem* WeaponItem;

	UPROPERTY()
		AFirearmWeaponItem* FirearmWeaponItem;

	UPROPERTY()
		ABaseWeaponItem* BeforeReplicateWeaponItem;

	UPROPERTY()
		ABaseWeaponItem* AfterLongActionWeaponItem;

	UPROPERTY(Replicated)
		EWeaponHoldState WeaponHoldState = EWeaponHoldState::Ready;


	// Etc

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TEnumAsByte<ECollisionChannel> ActionCollisionChannel = ECC_Visibility;

	//UPROPERTY()
	FActionDelegate AfterHolsterAction;
	//UPROPERTY()
	FActionDelegate BeforeLongAction;
	//UPROPERTY()
	FActionDelegate AfterLongAction;

	UPROPERTY()
		UAudioComponent* LongActionAudioComponent;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:


	// Items

	UFUNCTION()
		void UseWeapon1();

	UFUNCTION()
		void UseWeapon2();

	UFUNCTION()
		void UseWeapon3();

	UFUNCTION()
		void UseWeapon(ABaseWeaponItem* TheWeapon);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UseWeapon(ABaseWeaponItem* TheWeapon);

	//UFUNCTION()
		void DrawWeaponForAction(FActionDelegate const& Action);

	UFUNCTION()
		void UseItem(TSubclassOf<ABaseItem> ItemClass);

	UFUNCTION()
		void UseStackableItem(TSubclassOf<ABaseStackableItem> ItemClass);

	UFUNCTION()
		void UseBandage();

	UFUNCTION()
		void UseFirstAidKit();


	// Movement

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void CallJump();

	UFUNCTION()
		void CallStopJumping();

	UFUNCTION()
		void CallCrouch();

	UFUNCTION()
		void CallUnCrouch();

	UFUNCTION()
		void Walk();

	UFUNCTION()
		void StopWalking();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_WalkPressed(bool Pressed);

	UFUNCTION()
		void Sprint();

	UFUNCTION()
		void StopSprinting();

	UFUNCTION(Client, Reliable)
		void Client_SprintPressed(bool Pressed);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SprintPressed(bool Pressed);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_MoveForwardPressed(bool Pressed);

	UFUNCTION()
		float GetFloorMovementScale();

	UFUNCTION()
		bool ShouldCancelLongAction();

	UFUNCTION()
		void VerifySprinting();

	UFUNCTION()
		void UpdateMovingMode();

	UFUNCTION()
		void ApplyMovement();

	UFUNCTION()
		void UpdateLeanAngle(float DeltaTime);

	UFUNCTION()
		void UpdateLookRotatorForRemote();


	// Transformation

	UFUNCTION(NetMulticast, Unreliable)
		void Client_UpdateLookRotator(float Pitch, float Yaw, float Roll);

	UFUNCTION()
		void LeanLeft();

	UFUNCTION()
		void StopLeaningLeft();

	UFUNCTION()
		void LeanRight();

	UFUNCTION()
		void StopLeaningRight();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_LeanUpdated(float NewTargetLeanAngle);


	// Status

	UFUNCTION()
		void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(Client, Reliable)
		void Client_OnDamageReceived(float Damage, const UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
		void Die(AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void OnRep_IsDead();


	// HUD

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void CloseInventory();

	UFUNCTION()
		bool IsInventoryUIVisible();


	// Weapons

	UFUNCTION()
		void StartFiringPrimary();

	UFUNCTION()
		void StopFiringPrimary();

	UFUNCTION()
		void StartFiringSecondary();

	UFUNCTION()
		void StopFiringSecondary();

	UFUNCTION()
		void Aim();

	UFUNCTION()
		void StopAiming();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_AimPressed(bool Pressed);


	// Etc

	UFUNCTION(Client, Reliable)
		void Client_OnPossess();

	UFUNCTION(Client, Reliable)
		void Client_OnUnpossess();

	UFUNCTION()
		void DoAction();

	UFUNCTION()
		void StartLongAction(float TimeInSeconds, USoundBase* SoundToPlay);

	UFUNCTION()
		void FinishLongAction();

	UFUNCTION()
		void CancelLongAction();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_CancelLongAction();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SetLongActionStatus(float LongActionTime, bool TheIsDoingLongAction, USoundBase* SoundToPlay);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PickupItem(ABaseItem* Item);


public:

	// Movement

	UFUNCTION(Category = "Movement")
		float GetRunningSpeed() { return RunningSpeed; }

	UFUNCTION(BlueprintPure, Category = "Movement")
		EMovingMode GetMovingMode() { return MovingMode; }

	UFUNCTION(Category = "Movement")
		ETorsoAnimation GetTorsoAnimation();

	UFUNCTION(BlueprintPure, Category = "Movement")
		bool IsFalling();

	UFUNCTION(Category = "Movement")
		float GetMovingSpeedScale();

	UFUNCTION(Category = "Movement")
		bool ArmorAllowsRunning();

	UFUNCTION()
		void PlayAttackAnimation(bool Primary);

	UFUNCTION()
		void StopAttackAnimation();


	// Status

	UFUNCTION(BlueprintImplementableEvent)
		EBodyPart BoneToBodyPart(FName BoneName);

	UFUNCTION()
		bool GetIsDead() { return IsDead; };

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetHealth() { return Health; }

	UFUNCTION(BlueprintPure, Category = "Status")
		float GetPainStrength() { return PainStrength; }

	UFUNCTION(BlueprintCallable, Category = "Status")
		void Heal(int HealAmount);

	UFUNCTION()
		int GetCurrentFireMode();

	UFUNCTION()
		int GetWeaponAmmo();

	UFUNCTION()
		int GetWeaponAmmoInInventory();

	UFUNCTION()
		float GetWeaponInaccuracyRad();


	// Transformation

	UFUNCTION(BlueprintPure, Category="Transformation")
		FVector GetHorizontalVelocity();

	UFUNCTION(BlueprintPure, Category = "Transformation")
		FRotator GetLookRotator();

	UFUNCTION()
		FTransform GetCameraTransform();

	UFUNCTION()
		float GetLeanAngle() { return LeanAngle; };


	// HUD

	UFUNCTION()
		bool IsCustomUIVisible();

	UFUNCTION()
		void HideCustomUI();


	// Items

	UFUNCTION(BlueprintCallable, Category = "Items")
		void SetArmorItem(AArmorItem* TheArmorItem);

	UFUNCTION(BlueprintPure)
		AArmorItem* GetArmorItem() { return ArmorItem; }

	UFUNCTION()
		void OnRep_ArmorItem();

	UFUNCTION(BlueprintCallable, Category = "Items")
		void SetWeaponItem(ABaseWeaponItem* TheWeaponItem);

	UFUNCTION()
		ABaseWeaponItem* GetWeaponItem() { return WeaponItem; }

	UFUNCTION()
		void OnRep_WeaponItem();

	UFUNCTION()
		void ReloadWeapon();

	UFUNCTION()
		void SwitchWeaponFireMode();

	UFUNCTION()
		void SetIsReloading(bool IsReloading);

	UFUNCTION()
		EWeaponHoldState GetWeaponHoldState();

	UFUNCTION()
		void OnWeaponDrawFinished();

	UFUNCTION()
		void OnWeaponHolsterFinished();

	UFUNCTION()
		// We need this this keep items replication relevant when owner moves
		void UpdateItemsLocation();


	// Etc

	//UFUNCTION()
		void DoLongAction(float TimeInSeconds, FActionDelegate const& FuncOnStart, FActionDelegate const& FuncOnFinish, USoundBase* SoundToPlay);

	UFUNCTION(BlueprintPure)
		bool IsDoingLongAction();

	UFUNCTION()
		FHitResult LineTraceSingle(ECollisionChannel Channel, int Length, float OffsetAngle, bool ReturnMaterial, bool DrawDebug);

	UFUNCTION()
		TArray<FHitResult> LineTraceMulti(ECollisionChannel Channel, int Length, float OffsetAngle, bool ReturnMaterial, bool DrawDebug);

};
