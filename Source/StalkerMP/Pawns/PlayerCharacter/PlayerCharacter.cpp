// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "StalkerMP/StalkerMPGameInstance.h"
#include "StalkerMP/DataClasses/GlobalEnums.h"
#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/GameModes/BasePlayerState.h"
#include "StalkerMP/Pawns/ZoomableCameraManager.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacterUI.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacterAnimation.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerHandsAnimation.h"
#include "StalkerMP/Items/HUD/InventoryUI.h"
#include "StalkerMP/Items/DeadBodyBackpack.h"
#include "StalkerMP/Items/BaseItem.h"
#include "StalkerMP/Items/BaseStackableItem.h"
#include "StalkerMP/Items/ArmorItem.h"
#include "StalkerMP/Items/BaseWeaponItem.h"
#include "StalkerMP/Items/MeleeWeaponItem.h"
#include "StalkerMP/Items/FirearmWeaponItem.h"
#include "StalkerMP/Items/InventoryComponent.h"
#include "StalkerMP/DamageTypes/BulletDamageType.h"
#include "StalkerMP/DamageTypes/FallDamageType.h"

#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimBlueprint.h"
#include "Net/UnrealNetwork.h"


FOnPlayerCharacterDiedDelegate APlayerCharacter::OnPlayerCharacterDiedDelegate = FOnPlayerCharacterDiedDelegate();


// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	// Components

	NetCullDistanceSquared = 2500000000.0;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetMesh()->bCastHiddenShadow = true;
	GetMesh()->SetOwnerNoSee(true);

	GetCapsuleComponent()->SetCapsuleRadius(40);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->PrimaryComponentTick.bCanEverTick = false;
	PlayerCamera->PrimaryComponentTick.bStartWithTickEnabled = false;
	PlayerCamera->SetRelativeLocationAndRotation(FVector(0, 0, 80), FRotator(0, 0, 0));
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->SetupAttachment(GetCapsuleComponent());

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = true;
	FirstPersonMesh->bSelfShadowOnly = true;
	FirstPersonMesh->SetRelativeRotation(FRotator(0, 90, 0));
	FirstPersonMesh->SetRelativeLocation(FVector(0, 0, -20));
	FirstPersonMesh->SetupAttachment(PlayerCamera);

	ActionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	ActionSphere->PrimaryComponentTick.bCanEverTick = false;
	ActionSphere->PrimaryComponentTick.bStartWithTickEnabled = false;
	ActionSphere->SetCollisionProfileName(TEXT("ActionSource"));
	ActionSphere->CanCharacterStepUpOn = ECB_No;
	ActionSphere->SetGenerateOverlapEvents(true);
	ActionSphere->SetCanEverAffectNavigation(false);
	ActionSphere->SetSphereRadius(180);
	ActionSphere->SetRelativeLocation(FVector(0, 0, 60));
	ActionSphere->SetupAttachment(GetCapsuleComponent());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	InventoryComponent->PrimaryComponentTick.bCanEverTick = false;
	InventoryComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	InventoryComponent->SetupAttachment(RootComponent);


	// Movement

	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchRunningSpeed;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->MaxAcceleration = 1024;
	GetCharacterMovement()->BrakingFrictionFactor = 1;
	GetCharacterMovement()->BrakingDecelerationWalking = 1024;
	GetCharacterMovement()->SetWalkableFloorAngle(60);
	GetCharacterMovement()->bMaintainHorizontalGroundVelocity = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0);
	GetCharacterMovement()->CrouchedHalfHeight = 60;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_MOVE_FORWARD, this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_MOVE_BACKWARD, this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_STRAFE_LEFT, this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_STRAFE_RIGHT, this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_TURN_RIGHT, this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_LOOK_UP, this, &APlayerCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_JUMP, IE_Pressed, this, &APlayerCharacter::CallJump);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_JUMP, IE_Released, this, &APlayerCharacter::CallStopJumping);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_CROUCH, IE_Pressed, this, &APlayerCharacter::CallCrouch);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_CROUCH, IE_Released, this, &APlayerCharacter::CallUnCrouch);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_SPRINT, IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_SPRINT, IE_Released, this, &APlayerCharacter::StopSprinting);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_ACTION, IE_Pressed, this, &APlayerCharacter::DoAction);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_FIRE_PRIMARY, IE_Pressed, this, &APlayerCharacter::StartFiringPrimary);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_FIRE_PRIMARY, IE_Released, this, &APlayerCharacter::StopFiringPrimary);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_FIRE_SECONDARY, IE_Pressed, this, &APlayerCharacter::StartFiringSecondary);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_FIRE_SECONDARY, IE_Released, this, &APlayerCharacter::StopFiringSecondary);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_PICK_WEAPON_1, IE_Pressed, this, &APlayerCharacter::UseWeapon1);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_PICK_WEAPON_2, IE_Pressed, this, &APlayerCharacter::UseWeapon2);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_PICK_WEAPON_3, IE_Pressed, this, &APlayerCharacter::UseWeapon3);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_USE_BANDAGE, IE_Pressed, this, &APlayerCharacter::UseBandage);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_USE_FIRST_AID_KIT, IE_Pressed, this, &APlayerCharacter::UseFirstAidKit);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_RELOAD, IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_SWITCH_FIRE_MODE, IE_Pressed, this, &APlayerCharacter::SwitchWeaponFireMode);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_TOGGLE_INVENTORY, IE_Pressed, this, &APlayerCharacter::ToggleInventory);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerCharacter, MovingMode, COND_SkipOwner);
	DOREPLIFETIME(APlayerCharacter, ArmorItem);
	DOREPLIFETIME(APlayerCharacter, WeaponItem);
	DOREPLIFETIME_CONDITION(APlayerCharacter, Health, COND_OwnerOnly);
	DOREPLIFETIME(APlayerCharacter, WeaponHoldState);
	DOREPLIFETIME(APlayerCharacter, IsDead);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TScriptDelegate <FWeakObjectPtr> ReceiveAnyDamageDelegate;
		ReceiveAnyDamageDelegate.BindUFunction(this, "ReceiveDamage");
		OnTakeAnyDamage.AddUnique(ReceiveAnyDamageDelegate);

		GetWorld()->GetTimerManager().SetTimer(ItemLocationUpdateTimerHandle, this, &APlayerCharacter::UpdateItemsLocation, 1.0, true);
	}

	InventoryComponent->SetPlayerCharacter(this);
	InventoryComponent->SetPickupSphere(ActionSphere);
	GetMesh()->SetSkeletalMesh(DefaultCharacterMesh, false);
	FirstPersonMesh->SetSkeletalMesh(DefaultHandsMesh, false);

	StandCrouchCameraDiff = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - ((UCharacterMovementComponent*) GetMovementComponent())->CrouchedHalfHeight;

	UpdateMovingMode();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		if (WeaponItem)
		{
			WeaponItem->SetActorHiddenInGame(true);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	UpdateAngles(DeltaSeconds);

	if (JustLandedSpeedScale < 1)
	{
		JustLandedSpeedScale += DeltaSeconds / SecondsToRestoreFromFullFallStop;
		if (JustLandedSpeedScale > 1)
		{
			JustLandedSpeedScale = 1;
		}
	}

	if (HasAuthority())
	{
		UpdateLookRotatorForRemote();
		SetRemoteViewPitch(GetControlRotation().Pitch);

		if (ShouldCancelLongAction())
		{
			CancelLongAction();
		}
		VerifySprinting();
		ApplyMovement();
	}

	if (IsLocallyControlled())
	{
		if (OpenDeadBodyBackpack) {
			if (InventoryUI->IsDeadBodyInventoryUIVisible())
			{
				if (!ActionSphere->IsOverlappingActor(OpenDeadBodyBackpack))
				{
					OpenDeadBodyBackpack->SetInventoryUI(nullptr);
					InventoryUI->SetDeadBodyBackpack(nullptr);
				}
			}
			else
			{
				if (ActionSphere->IsOverlappingActor(OpenDeadBodyBackpack))
				{
					OpenDeadBodyBackpack->SetInventoryUI(InventoryUI);
					InventoryUI->SetDeadBodyBackpack(OpenDeadBodyBackpack);
				}
			}
		}

		if (PainStrength > 0)
		{
			PainStrength -= PAIN_DECREASE_SPEED * DeltaSeconds;

			if (PainStrength < 0) {
				PainStrength = 0;
			}
		}

		if (ShouldCancelLongAction())
		{
			CancelLongAction();
		}
		VerifySprinting();
		ApplyMovement();
	}

	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerName = NewController->PlayerState->GetPlayerName();
	Client_OnPossess();
	InventoryComponent->OnPossess();
}

void APlayerCharacter::UnPossessed()
{
	Client_OnUnpossess();

	GetMovementComponent()->StopMovementImmediately();

	Super::UnPossessed();
}

void APlayerCharacter::AddControllerYawInput(float Val)
{
	if (GetController())
	{
		Super::AddControllerYawInput(Val * GetController<ABasePlayerController>()->GetHorizontalSensitivity());
	}
}

void APlayerCharacter::AddControllerPitchInput(float Val)
{
	if (GetController())
	{
		Super::AddControllerPitchInput(Val * GetController<ABasePlayerController>()->GetVerticalSensitivity());
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	float VerticalVelocity = GetVelocity().Z
		+ MinVerticalVelocityToGetFallDamage
		+ (ArmorItem ? ArmorItem->GetFallVelocityReduction() : 0);

	if (HasAuthority())
	{
		if (VerticalVelocity < 0)
		{
			VerticalVelocity /= -10;

			TakeDamage(VerticalVelocity, FDamageEvent(UFallDamageType::StaticClass()), nullptr, nullptr);

			if (FirearmWeaponItem && WeaponHoldState == EWeaponHoldState::Reloading)
			{
				FirearmWeaponItem->CancelReload();
			}
		}
	}

	if (VerticalVelocity < 0)
	{
		JustLandedSpeedScale = MinJustLandedSpeedScale;
	}
	else
	{
		JustLandedSpeedScale = VerticalVelocity / MinVerticalVelocityToGetFallDamage;
		if (JustLandedSpeedScale > 1)
		{
			JustLandedSpeedScale = 1;
		}
	}

	((UPlayerCharacterAnimation*)GetMesh()->GetAnimInstance())->OnLanded();
}



// ------------------------------------------------------------------------- Movement --------------------------------------------------------------------------

float APlayerCharacter::GetFloorMovementScale()
{
	return 1;
}

bool APlayerCharacter::ShouldCancelLongAction()
{
	return bSprintActionPressed && GetVelocity().Size() >= (bIsCrouched ? CrouchCancelLongActionSpeedThreshold : CancelLongActionSpeedThreshold);
}

void APlayerCharacter::VerifySprinting()
{
	if (MovingMode == EMovingMode::Sprinting || MovingMode == EMovingMode::SprintingToSide)
	{
		if (MovingMode == EMovingMode::Sprinting && !bMoveForwardPressed)
		{
			MovingMode = EMovingMode::SprintingToSide;
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		else if (MovingMode == EMovingMode::SprintingToSide && bMoveForwardPressed)
		{
			MovingMode = EMovingMode::Sprinting;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}
}

void APlayerCharacter::UpdateMovingMode()
{
	if (IsDoingLongAction())
	{
		if (bSprintActionPressed && ArmorAllowsRunning())
		{
			MovingMode = EMovingMode::Sprinting;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		else
		{
			MovingMode = EMovingMode::Walking;
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else if (bAimActionPressed)
	{
		MovingMode = EMovingMode::Aimimg;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if (bSprintActionPressed && WeaponHoldState != EWeaponHoldState::Reloading && ArmorAllowsRunning())
	{
		MovingMode = EMovingMode::Sprinting;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		MovingMode = EMovingMode::Running;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void APlayerCharacter::ApplyMovement()
{
	switch (MovingMode)
	{
	case EMovingMode::Aimimg:
	{
		GetCharacterMovement()->MaxWalkSpeed = AimimgSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchAimimgSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		break;
	}
	case EMovingMode::Walking:
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchWalkingSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		break;
	}
	case EMovingMode::Running:
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchRunningSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		break;
	}
	case EMovingMode::Sprinting:
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSprintingSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		break;
	}
	case EMovingMode::SprintingToSide:
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchRunningSpeed * GetFloorMovementScale() * JustLandedSpeedScale;
		break;
	}
	}
}

void APlayerCharacter::UpdateAngles(float DeltaTime)
{

	//if (IsLocallyControlled())
	{
		bool CameraOffsetChanged = false;
		FVector CameraLocationOffset = UKismetMathLibrary::VLerp(StandingCameraOffset, CrouchingCameraOffset, CameraCrouchLerp);
		if (CameraCrouchLerp < TargetCameraCrouchLerp)
		{
			float LerpChange = DeltaTime / SecondsToCrouch;
			CameraCrouchLerp += LerpChange;
			if (CameraCrouchLerp > TargetCameraCrouchLerp)
			{
				CameraCrouchLerp = TargetCameraCrouchLerp;
			}

			CameraLocationOffset = UKismetMathLibrary::VLerp(StandingCameraOffset, CrouchingCameraOffset, CameraCrouchLerp);
			CameraOffsetChanged = true;
		}
		else if (CameraCrouchLerp > TargetCameraCrouchLerp)
		{
			float LerpChange = DeltaTime / SecondsToCrouch;
			CameraCrouchLerp -= LerpChange;
			if (CameraCrouchLerp < TargetCameraCrouchLerp)
			{
				CameraCrouchLerp = TargetCameraCrouchLerp;
			}

			CameraLocationOffset = UKismetMathLibrary::VLerp(StandingCameraOffset, CrouchingCameraOffset, CameraCrouchLerp);
			CameraOffsetChanged = true;
		}

		if (CameraOffsetChanged)
		{
			CameraLocationOffset = FVector(0, CameraLocationOffset.Y, CameraLocationOffset.Z);

			if (bIsCrouched)
			{
				CameraLocationOffset += FVector(0, 0, StandCrouchCameraDiff * (1 - CameraCrouchLerp));
			}
			else
			{
				CameraLocationOffset -= FVector(0, 0, StandCrouchCameraDiff * CameraCrouchLerp);
			}
			PlayerCamera->SetRelativeLocation(CameraLocationOffset);
		}
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller && !IsDead)
	{
		if (Value != 0.0f)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}

		if (!bMoveForwardPressed && Value > 0) {
			bMoveForwardPressed = true;
			Server_MoveForwardPressed(true);
		}
		else if (bMoveForwardPressed && Value < 0) {
			bMoveForwardPressed = false;
			Server_MoveForwardPressed(false);
		}
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller && !IsDead)
	{
		if (Value != 0.0f)
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

void APlayerCharacter::CallJump()
{
	if (Controller && !IsDead)
	{
		Jump();
	}
}

void APlayerCharacter::CallStopJumping()
{
	StopJumping();
}

void APlayerCharacter::CallCrouch()
{
	bCrouchActionPressed = true;
	Crouch(true);
	TargetCameraCrouchLerp = 1;
	UpdateMovingMode();
}

void APlayerCharacter::CallUnCrouch()
{
	bCrouchActionPressed = false;
	UnCrouch(true);
	TargetCameraCrouchLerp = 0;
	UpdateMovingMode();
}

void APlayerCharacter::Sprint()
{
	Client_SprintPressed_Implementation(true);
	Server_SprintPressed(true);
}

void APlayerCharacter::StopSprinting()
{
	Client_SprintPressed_Implementation(false);
	Server_SprintPressed(false);
}

void APlayerCharacter::Client_SprintPressed_Implementation(bool Pressed)
{
	bSprintActionPressed = Pressed;
	UpdateMovingMode();
}

void APlayerCharacter::Server_SprintPressed_Implementation(bool Pressed)
{
	bSprintActionPressed = Pressed;
	UpdateMovingMode();
}

bool APlayerCharacter::Server_SprintPressed_Validate(bool Pressed)
{
	// We don't validate yet
	return true;
}

void APlayerCharacter::Server_MoveForwardPressed_Implementation(bool Pressed)
{
	bMoveForwardPressed = Pressed;
}

bool APlayerCharacter::Server_MoveForwardPressed_Validate(bool Pressed)
{
	// We don't validate yet
	return true;
}

bool APlayerCharacter::IsFalling()
{
	if (GetCharacterMovement())
	{
		return (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling);
	}

	return false;
}

float APlayerCharacter::GetMovingSpeedScale()
{
	return GetVelocity().Size() / RunningSpeed;
}

bool APlayerCharacter::ArmorAllowsRunning()
{
	if (ArmorItem)
	{
		return ArmorItem->GetCanRun();
	}

	return true;
}

void APlayerCharacter::PlayAttackAnimation(bool Primary)
{
	((UPlayerCharacterAnimation*)GetMesh()->GetAnimInstance())->PlayAttackAnimation(Primary);
}

void APlayerCharacter::StopAttackAnimation()
{
	((UPlayerCharacterAnimation*)GetMesh()->GetAnimInstance())->StopAttackAnimation();
}




// ------------------------------------------------------------------------- Status --------------------------------------------------------------------------

void APlayerCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		Client_OnDamageReceived(Damage, DamageType, DamageCauser);

		Health -= Damage;

		if (Health <= 0)
		{
			Health = 0;

			Die(InstigatedBy, DamageCauser);
		}
	}
}

void APlayerCharacter::Client_OnDamageReceived_Implementation(float Damage, const class UDamageType* DamageType, AActor* DamageCauser)
{
	if (!IsDead)
	{
		PainStrength += Damage / MAX_DAMAGE_FOR_MAX_PAIN;
		if (PainStrength > 1)
		{
			PainStrength = 1;
		}
		if (PainStrength < 0.2)
		{
			PainStrength = 0.2;
		}
	}
}

void APlayerCharacter::Die(class AController* InstigatedBy, AActor* DamageCauser)
{
	SetCanBeDamaged(false);
	IsDead = true;
	OnRep_IsDead();

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FRotator SpawnRotation = GetActorRotation();
	SpawnRotation.Yaw += 90;

	FTransform SpawnTransform = FTransform();
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	DeadBodyBackpack = GetWorld()->SpawnActor<ADeadBodyBackpack>(DeadBodyBackpackClass, SpawnTransform, SpawnInfo);
	InventoryComponent->FillDeadBodyBackpack(DeadBodyBackpack);

	OnPlayerCharacterDiedDelegate.Broadcast(this, GetController(), InstigatedBy, DamageCauser, DeadBodyBackpack);
}

void APlayerCharacter::OnRep_IsDead()
{
	CancelLongAction();

	if (!HasAuthority())
	{
		USoundBase* DeathSound = DeathSounds[UKismetMathLibrary::RandomInteger(DeathSounds.Num())];
		UGameplayStatics::SpawnSoundAttached(DeathSound, GetMesh(), MounthSocketName,
			FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, true);

		if (WeaponItem)
		{
			WeaponItem->StopFiring();
			WeaponItem->GetMesh()->SetVisibility(true);
			WeaponItem->GetFPPMesh()->SetVisibility(false);
		}
	}

	GetCapsuleComponent()->SetCollisionProfileName("Spectator");
	GetMesh()->SetCollisionProfileName("DeadCharacter");
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	GetMovementComponent()->StopMovementImmediately();

	if (FirearmWeaponItem) {
		FirearmWeaponItem->CancelReload();
	}
}

void APlayerCharacter::Heal(int HealAmount)
{
	Health += HealAmount;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

ETorsoAnimation APlayerCharacter::GetTorsoAnimation()
{
	if (WeaponItem)
	{
		return WeaponItem->GetTorsoAnimation();
	}

	return ETorsoAnimation::Torso0;
}

int APlayerCharacter::GetCurrentFireMode()
{
	if (FirearmWeaponItem)
	{
		return FirearmWeaponItem->GetFireRoundMode();
	}

	return 0;
}

int APlayerCharacter::GetWeaponAmmo()
{
	if (FirearmWeaponItem)
	{
		return FirearmWeaponItem->GetAmmo();
	}

	return 0;
}

int APlayerCharacter::GetWeaponAmmoInInventory()
{
	if (WeaponItem)
	{
		if (FirearmWeaponItem)
		{
			return InventoryComponent->Backpack_CountStackableItemsOfType(FirearmWeaponItem->AmmoItemClass);
		}

		return -1;
	}

	return -2;
}

float APlayerCharacter::GetWeaponInaccuracyRad()
{
	if (FirearmWeaponItem)
	{
		return FirearmWeaponItem->GetInaccuracyRad();
	}
	else
	{
		return 0;
	}
}



// ------------------------------------------------------------------------- Transformatiom --------------------------------------------------------------------------

void APlayerCharacter::UpdateLookRotatorForRemote()
{
	FRotator ControlRotation = GetControlRotation();
	Client_UpdateLookRotator(ControlRotation.Pitch, ControlRotation.Yaw, ControlRotation.Roll);
}

void APlayerCharacter::Client_UpdateLookRotator_Implementation(float Pitch, float Yaw, float Roll)
{
	if (!IsLocallyControlled())
	{
		LookRotator.Pitch = Pitch;
		LookRotator.Yaw = Yaw;
		LookRotator.Roll = Roll;
	}
}

FTransform APlayerCharacter::GetCameraTransform()
{
	FTransform Transform = PlayerCamera->GetComponentTransform();
	FRotator Rotator = GetControlRotation();
	Transform.SetRotation(Rotator.Quaternion());
	return Transform;
}

FVector APlayerCharacter::GetHorizontalVelocity()
{
	const FVector Vector = GetVelocity();
	return FVector(Vector.X, Vector.Y, 0);
}

FRotator APlayerCharacter::GetLookRotator()
{
	if (IsLocallyControlled())
	{
		return GetControlRotation();
	}
	else
	{
		return LookRotator;
	}
}




// ------------------------------------------------------------------------- HUD --------------------------------------------------------------------------

void APlayerCharacter::ToggleInventory()
{
	if (IsLocallyControlled())
	{
		if (IsInventoryUIVisible())
		{
			CloseInventory();
		}
		else
		{
			OpenInventory();
		}
	}
}

void APlayerCharacter::OpenInventory()
{
	if (GetController<ABasePlayerController>()->IsGameMenuVisible())
	{
		return;
	}

	GetController<ABasePlayerController>()->HideAllUI(false);
	InventoryUI->SetVisibility(ESlateVisibility::Visible);
	GetController<ABasePlayerController>()->UpdateMouseControl();
}

void APlayerCharacter::CloseInventory()
{
	if (OpenDeadBodyBackpack)
	{
		OpenDeadBodyBackpack->SetInventoryUI(nullptr);
	}
	OpenDeadBodyBackpack = nullptr;
	InventoryUI->SetDeadBodyBackpack(nullptr);
	InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
	GetController<ABasePlayerController>()->UpdateMouseControl();
}

bool APlayerCharacter::IsInventoryUIVisible()
{
	return (InventoryUI->GetVisibility() == ESlateVisibility::Visible);
}

bool APlayerCharacter::IsCustomUIVisible()
{
	return IsInventoryUIVisible();
}

void APlayerCharacter::HideCustomUI()
{
	CloseInventory();
}





// ------------------------------------------------------------------------- Items --------------------------------------------------------------------------

void APlayerCharacter::SetArmorItem(AArmorItem* TheArmorItem)
{
	ArmorItem = TheArmorItem;

	UpdateMovingMode();
}

void APlayerCharacter::OnRep_ArmorItem()
{
	if (ArmorItem)
	{
		GetMesh()->SetSkeletalMesh(ArmorItem->GetCharacterMesh(), false);
		GetMesh()->SetPhysMaterialOverride(ArmorItem->GetCharacterMesh()->Materials[0].MaterialInterface->GetPhysicalMaterial());
		if (IsLocallyControlled())
		{
			FirstPersonMesh->SetSkeletalMesh(ArmorItem->GetFirstPersonMesh(), false);
		}
	}
	else
	{
		GetMesh()->SetSkeletalMesh(DefaultCharacterMesh, false);
		GetMesh()->SetPhysMaterialOverride(DefaultCharacterMesh->Materials[0].MaterialInterface->GetPhysicalMaterial());
		if (IsLocallyControlled())
		{
			FirstPersonMesh->SetSkeletalMesh(DefaultHandsMesh, false);
		}
	}

	UpdateMovingMode();
}

void APlayerCharacter::SetWeaponItem(ABaseWeaponItem* TheWeaponItem)
{
	if (WeaponItem != TheWeaponItem)
	{
		WeaponItem = TheWeaponItem;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("OnRep_WeaponItem"));
		GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
	}
}

void APlayerCharacter::OnRep_WeaponItem()
{
	WeaponHoldState = EWeaponHoldState::Showing;

	if (AFirearmWeaponItem* FirearmItem = dynamic_cast<AFirearmWeaponItem*>(WeaponItem)) {
		FirearmWeaponItem = FirearmItem;
	}
	else {
		FirearmWeaponItem = nullptr;
	}

	if (WeaponItem)
	{
		if (!HasAuthority())
		{
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			WeaponItem->AttachToComponent(GetMesh(), AttachmentTransformRules, WeaponSocketName);
			WeaponItem->SetAnimState(EWeaponAnimState::Hold);

			if (IsLocallyControlled())
			{
				WeaponItem->SetActorTickEnabled(true);
				WeaponItem->GetMesh()->SetVisibility(false);
				WeaponItem->GetMesh()->bCastHiddenShadow = true;
				WeaponItem->GetFPPMesh()->SetComponentTickEnabled(true);
				WeaponItem->GetFPPMesh()->bRenderStatic = false;
				WeaponItem->GetFPPMesh()->AttachToComponent(FirstPersonMesh, AttachmentTransformRules, WeaponSocketName);
				WeaponItem->GetFPPMesh()->SetVisibility(true);
			}
		}

		((UPlayerCharacterAnimation*)GetMesh()->GetAnimInstance())->SetWeaponTorso(
			WeaponItem->GetTorsoAnimation(), FirearmWeaponItem ? FirearmWeaponItem->GetReloadTime() : 1, WeaponItem->GetDrawTime(), WeaponItem->GetHolsterTime());
		FirstPersonMesh->SetAnimInstanceClass(WeaponItem->HandsAnimationClass.Get());
		WeaponItem->SetHandsAnimation((UPlayerHandsAnimation*) FirstPersonMesh->GetAnimInstance());
	}
	else
	{
		WeaponHoldState = EWeaponHoldState::Ready;

		((UPlayerCharacterAnimation*)GetMesh()->GetAnimInstance())->SetWeaponTorso(ETorsoAnimation::Torso0, 1, 1, 1);
		FirstPersonMesh->SetAnimInstanceClass(nullptr);
	}

	if (BeforeReplicateWeaponItem)
	{
		if (!HasAuthority())
		{
			if (BeforeReplicateWeaponItem->GetAttachParentActor() == this)
			{
				FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
				BeforeReplicateWeaponItem->DetachFromActor(DetachmentTransformRules);
				BeforeReplicateWeaponItem->SetAnimState(EWeaponAnimState::OnGround);

				if (IsLocallyControlled())
				{
					BeforeReplicateWeaponItem->SetActorTickEnabled(false);
					BeforeReplicateWeaponItem->GetMesh()->bCastHiddenShadow = false;
					BeforeReplicateWeaponItem->GetMesh()->SetVisibility(true);
					BeforeReplicateWeaponItem->GetFPPMesh()->SetComponentTickEnabled(false);
					BeforeReplicateWeaponItem->GetFPPMesh()->bRenderStatic = true;
					BeforeReplicateWeaponItem->GetFPPMesh()->DetachFromComponent(DetachmentTransformRules);
					BeforeReplicateWeaponItem->GetFPPMesh()->SetVisibility(false);
				}
			}
		}
	}

	BeforeReplicateWeaponItem = WeaponItem;
}

void APlayerCharacter::ReloadWeapon()
{
	if (FirearmWeaponItem && GetVelocity().Z > -MinVerticalVelocityToGetFallDamage && WeaponHoldState == EWeaponHoldState::Ready)
	{
		FirearmWeaponItem->Reload();
	}
}

void APlayerCharacter::SwitchWeaponFireMode()
{
	if (FirearmWeaponItem)
	{
		FirearmWeaponItem->SwitchFireRoundMode();
	}
}

void APlayerCharacter::UpdateItemsLocation()
{
	InventoryComponent->UpdateItemsLocation(GetActorLocation());
}

void APlayerCharacter::SetIsReloading(bool IsReloading)
{
	if (IsReloading)
	{
		if (IsLocallyControlled())
		{
			StopAiming();
		}

		WeaponHoldState = EWeaponHoldState::Reloading;
	}
	else if (WeaponHoldState == EWeaponHoldState::Reloading)
	{
		WeaponHoldState = EWeaponHoldState::Ready;
	}

	UpdateMovingMode();
}

EWeaponHoldState APlayerCharacter::GetWeaponHoldState()
{
	return WeaponHoldState;
}

void APlayerCharacter::OnWeaponDrawFinished()
{
	WeaponHoldState = EWeaponHoldState::Ready;
}

void APlayerCharacter::OnWeaponHolsterFinished()
{
	AfterHolsterAction.ExecuteIfBound();
	AfterHolsterAction = FActionDelegate();
}




// ------------------------------------------------------------------------- Weapons --------------------------------------------------------------------------

void APlayerCharacter::StartFiringPrimary()
{
	if (WeaponItem)
	{
		if (WeaponHoldState == EWeaponHoldState::Ready)
		{
			if (GetMovingMode() == EMovingMode::Sprinting)
			{
				StopSprinting();
			}

			WeaponItem->StartFiring();
		}
	}
}

void APlayerCharacter::StopFiringPrimary()
{
	if (WeaponItem)
	{
		WeaponItem->StopFiring();
	}
}

void APlayerCharacter::StartFiringSecondary()
{
	Aim();
}

void APlayerCharacter::StopFiringSecondary()
{
	StopAiming();
}

void APlayerCharacter::Aim()
{
	if (FirearmWeaponItem && WeaponHoldState == EWeaponHoldState::Ready && !FirearmWeaponItem->GetIsReloading())
	{
		bAimActionPressed = true;
		UpdateMovingMode();
		Server_AimPressed(true);

		AZoomableCameraManager* CameraManager = Cast<AZoomableCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		if (CameraManager)
		{
			CameraManager->SetZoomScale(AimFieldOfViewScale);
		}
	}
}

void APlayerCharacter::StopAiming()
{
	bAimActionPressed = false;
	UpdateMovingMode();
	Server_AimPressed(false);

	AZoomableCameraManager* CameraManager = Cast<AZoomableCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	if (CameraManager)
	{
		CameraManager->SetZoomScale(1.0);
	}
}

void APlayerCharacter::Server_AimPressed_Implementation(bool Pressed)
{
	bAimActionPressed = Pressed;
	UpdateMovingMode();
}

bool APlayerCharacter::Server_AimPressed_Validate(bool Pressed)
{
	// We don't validate yet
	return true;
}

void APlayerCharacter::UseWeapon1()
{
	UseWeapon(InventoryComponent->GetWeapon1());
}

void APlayerCharacter::UseWeapon2()
{
	UseWeapon(InventoryComponent->GetWeapon2());
}

void APlayerCharacter::UseWeapon3()
{
	UseWeapon(InventoryComponent->GetWeapon3());
}

void APlayerCharacter::UseWeapon(ABaseWeaponItem* TheWeapon)
{
	if (TheWeapon != nullptr)
	{
		if (IsLocallyControlled())
		{
			Server_UseWeapon(TheWeapon);
		}

		if (IsDoingLongAction())
		{
			AfterLongActionWeaponItem = TheWeapon;
			CancelLongAction();

			return;
		}

		if (TheWeapon == WeaponItem)
		{
			if (WeaponHoldState == EWeaponHoldState::Hiding)
			{
				WeaponHoldState = EWeaponHoldState::Showing;
				AfterHolsterAction = FActionDelegate();
			}
		}
		else
		{
			if (WeaponItem)
			{
				WeaponHoldState = EWeaponHoldState::Hiding;
				AfterHolsterAction = FActionDelegate();
				
				if (HasAuthority())
				{
					FActionDelegate UseWeaponDelegate;
					UseWeaponDelegate.BindUFunction(InventoryComponent, FName("Server_UseWeapon"), TheWeapon);
					DrawWeaponForAction(UseWeaponDelegate);
				}
			}
			else
			{
				if (HasAuthority())
				{
					InventoryComponent->Server_UseWeapon(TheWeapon);
				}
			}
		}
	}
}

void APlayerCharacter::Server_UseWeapon_Implementation(ABaseWeaponItem* TheWeapon)
{
	UseWeapon(TheWeapon);
}

bool APlayerCharacter::Server_UseWeapon_Validate(ABaseWeaponItem* TheWeapon)
{
	return true;
}

void APlayerCharacter::DrawWeaponForAction(FActionDelegate const& Action)
{
	StopAiming();
	if (WeaponItem)
	{
		WeaponItem->StopFiring();
		if (FirearmWeaponItem)
		{
			FirearmWeaponItem->CancelReload();
		}

		WeaponHoldState = EWeaponHoldState::Hiding;
		AfterHolsterAction = Action;
	}
	else
	{
		AfterHolsterAction = Action;
		OnWeaponHolsterFinished();
	}
}

void APlayerCharacter::UseItem(TSubclassOf<ABaseItem> ItemClass)
{
	TArray<ABaseItem*> ItemsInInventory;
	InventoryComponent->Backpack_GetItemsOfType(ItemClass, ItemsInInventory);
	if (ItemsInInventory.Num() > 0 && ItemsInInventory[0]->IsCanBeUsed())
	{
		ItemsInInventory[0]->Server_Use();
	}
}

void APlayerCharacter::UseStackableItem(TSubclassOf<ABaseStackableItem> ItemClass)
{
	TArray<ABaseStackableItem*> ItemsInInventory;
	InventoryComponent->Backpack_GetStackableItemsOfType(ItemClass, ItemsInInventory);
	if (ItemsInInventory.Num() > 0 && ItemsInInventory[0]->IsCanBeUsed())
	{
		ItemsInInventory[0]->Server_Use();
	}
}

void APlayerCharacter::DoLongAction(float TimeInSeconds, FActionDelegate const& FuncOnStart, FActionDelegate const& FuncOnFinish, USoundBase* UseSound, USoundBase* AfterUseSound)
{
	if (!IsDoingLongAction())
	{
		Client_SprintPressed(false);
		Server_SprintPressed_Implementation(false);
		BeforeLongAction = FuncOnStart;
		AfterLongAction = FuncOnFinish;
		AfterLongActionWeaponItem = WeaponItem;
		FActionDelegate StartLongActionDelegate;
		StartLongActionDelegate.BindUFunction(this, FName("StartLongAction"), TimeInSeconds, UseSound, AfterUseSound);
		DrawWeaponForAction(StartLongActionDelegate);
	}
}

void APlayerCharacter::StartLongAction(float TimeInSeconds, USoundBase* UseSound, USoundBase* AfterUseSound)
{
	if (HasAuthority())
	{
		InventoryComponent->Server_UseWeapon(nullptr);
	}

	BeforeLongAction.ExecuteIfBound();
	BeforeLongAction = FActionDelegate();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("FinishLongAction"), AfterUseSound);
	GetWorld()->GetTimerManager().SetTimer(LongActionTimerHandle, TimerDelegate, TimeInSeconds, false);

	Multicast_SetLongActionStatus(TimeInSeconds, true, UseSound);
}

void APlayerCharacter::FinishLongAction(USoundBase* AfterUseSound)
{
	AfterLongAction.ExecuteIfBound();
	AfterLongAction = FActionDelegate();
	Multicast_SetLongActionStatus(0, false, nullptr);
	Multicast_PlaySound(AfterUseSound);

	if (HasAuthority())
	{
		InventoryComponent->Server_UseWeapon(AfterLongActionWeaponItem);
	}

	AfterLongActionWeaponItem = nullptr;
}

void APlayerCharacter::CancelLongAction()
{
	if (IsDoingLongAction())
	{
		Multicast_SetLongActionStatus(0, false, nullptr);
		BeforeLongAction = FActionDelegate();
		AfterLongAction = FActionDelegate();

		GetWorld()->GetTimerManager().ClearTimer(LongActionTimerHandle);

		if (HasAuthority())
		{
			InventoryComponent->Server_UseWeapon(AfterLongActionWeaponItem);
		}

		AfterLongActionWeaponItem = nullptr;
	}
}

void APlayerCharacter::Server_CancelLongAction_Implementation()
{
	CancelLongAction();
}

bool APlayerCharacter::Server_CancelLongAction_Validate()
{
	return true;
}

bool APlayerCharacter::IsDoingLongAction()
{
	return DoingLongAction;
}

void APlayerCharacter::Multicast_SetLongActionStatus_Implementation(float LongActionTime, bool TheIsDoingLongAction, USoundBase* UseSound)
{
	DoingLongAction = TheIsDoingLongAction;
	UpdateMovingMode();

	if (TheIsDoingLongAction)
	{
		if (!HasAuthority())
		{
			LongActionAudioComponent = UGameplayStatics::SpawnSoundAttached(UseSound, GetMesh(), FName(""),
				FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, true);
		}
	}
	else
	{
		if (LongActionAudioComponent)
		{
			LongActionAudioComponent->Stop();
			LongActionAudioComponent = nullptr;
		}

		if (WeaponItem)
		{
			WeaponHoldState = EWeaponHoldState::Showing;
		}
		else
		{
			WeaponHoldState = EWeaponHoldState::Ready;
		}
	}

	if (IsLocallyControlled())
	{
		PlayerCharacterUI->SetIsDoingLongAction(LongActionTime, TheIsDoingLongAction);
	}
}

void APlayerCharacter::UseBandage()
{
	UseStackableItem(BandageItemClass);
}

void APlayerCharacter::UseFirstAidKit()
{
	UseStackableItem(FirstAidKitItemClass);
}

void APlayerCharacter::Client_PlaySound_Implementation(USoundBase* SoundToPlay)
{
	UGameplayStatics::SpawnSound2D(GetWorld(), SoundToPlay, 1);
}

void APlayerCharacter::Multicast_PlaySound_Implementation(USoundBase* Sound)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, GetActorLocation(), FRotator::ZeroRotator, 1.0, 1.0, 0.f, nullptr, nullptr, true);
}

void APlayerCharacter::Multicast_PlaySoundAttached_Implementation(USoundBase* Sound)
{
	UGameplayStatics::SpawnSoundAttached(Sound, GetCapsuleComponent(), FName(""),
		FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, true);
}




// ------------------------------------------------------------------------- Etc --------------------------------------------------------------------------

void APlayerCharacter::Client_OnPossess_Implementation()
{
	if (IsLocallyControlled())
	{
		BaseFirstPersonMeshTransform = FirstPersonMesh->GetRelativeTransform();

		if (PlayerCharacterUI) {
			PlayerCharacterUI->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PlayerCharacterUI = CreateWidget<UPlayerCharacterUI>(GetController<ABasePlayerController>(), PlayerCharacterUIClass);
			PlayerCharacterUI->SetPlayerCharacter(this);
			PlayerCharacterUI->AddToViewport(ABasePlayerController::PLAYER_CHARACTER_UI_ZORDER);

			InventoryUI = CreateWidget<UInventoryUI>(GetController<ABasePlayerController>(), InventoryUIClass);
			InventoryUI->SetInventoryComponent(InventoryComponent);
			InventoryUI->AddToViewport(ABasePlayerController::PLAYER_CHARACTER_INVENTORY_ZORDER);
			InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
			InventoryComponent->SetInventoryUI(InventoryUI);
		}
	}
}

void APlayerCharacter::Client_OnUnpossess_Implementation()
{
	if (IsLocallyControlled())
	{
		if (PlayerCharacterUI) {
			PlayerCharacterUI->RemoveFromParent();
			PlayerCharacterUI = nullptr;
			InventoryUI->RemoveFromParent();
			InventoryUI = nullptr;
		}

		AZoomableCameraManager* CameraManager = Cast<AZoomableCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		if (CameraManager)
		{
			CameraManager->SetZoomScale(1.0);
		}
	}
}

void APlayerCharacter::DoAction()
{
	FHitResult HitResult = LineTraceSingle(ActionCollisionChannel, ActionSphere->GetScaledSphereRadius(), 0, false, false);

	if (ABaseItem* Item = dynamic_cast<ABaseItem*>(HitResult.GetActor())) {
		if (InventoryComponent->CanPickupItem(Item))
		{
			Server_PickupItem(Item);
		}
		else
		{
			//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Can't pickup item");
		}
	}
	else {
		if (ADeadBodyBackpack* TheDeadBodyBackpack = dynamic_cast<ADeadBodyBackpack*>(HitResult.GetActor())) {
			if (ActionSphere->IsOverlappingActor(TheDeadBodyBackpack))
			{
				if (!OpenDeadBodyBackpack)
				{
					OpenDeadBodyBackpack = TheDeadBodyBackpack;
					OpenInventory();
				}
				else
				{
					OpenDeadBodyBackpack = nullptr;
					CloseInventory();
				}
			}
		}
	}
}

void APlayerCharacter::Server_PickupItem_Implementation(ABaseItem* Item)
{
	InventoryComponent->Server_PickupItem(Item);
}

bool APlayerCharacter::Server_PickupItem_Validate(ABaseItem* Item)
{
	return true;
}

FHitResult APlayerCharacter::LineTraceSingle(ECollisionChannel Channel, int Length, float OffsetAngle, bool ReturnMaterial, bool DrawDebug)
{
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector End = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * Length;
	if (OffsetAngle != 0)
	{
		End = FRotator(0, OffsetAngle, 0).RotateVector(End - Start) + Start;
	}

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.bReturnPhysicalMaterial = ReturnMaterial;
	FCollisionResponseParams CollisionResponseParams = FCollisionResponseParams();

	if (DrawDebug)
	{
		const FName TraceTag("ActionTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, Channel, CollisionQueryParams, CollisionResponseParams);

	return HitResult;
}

TArray<FHitResult> APlayerCharacter::LineTraceMulti(ECollisionChannel Channel, int Length, float OffsetAngle, bool ReturnMaterial, bool DrawDebug)
{
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector End = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * Length;
	if (OffsetAngle != 0)
	{
		End = FRotator(0, OffsetAngle, 0).RotateVector(End - Start) + Start;
	}

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.bReturnPhysicalMaterial = ReturnMaterial;
	FCollisionResponseParams CollisionResponseParams = FCollisionResponseParams();

	if (DrawDebug)
	{
		const FName TraceTag("ActionTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionQueryParams.TraceTag = TraceTag;
	}

	TArray<FHitResult> HitResults;

	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, Channel, CollisionQueryParams, CollisionResponseParams);

	return HitResults;
}
