// Fill out your copyright notice in the Description page of Project Settings.

#include "Spectator.h"

#include "StalkerMP/GameModes/BasePlayerController.h"

#include "Engine/CollisionProfile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ASpectator::ASpectator()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	NetPriority = 3.0f;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName("Spectator");
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;
	RootComponent = CollisionComponent;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->UpdatedComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void ASpectator::BeginPlay()
{
	Super::BeginPlay();

	// Movement
	if (HasAuthority())
	{
		FloatingPawnMovement->MaxSpeed = SprintingSpeed;
	}
	else
	{
		FloatingPawnMovement->MaxSpeed = RunningSpeed;
	}
}

// Called to bind functionality to input
void ASpectator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_MOVE_FORWARD, this, &ASpectator::MoveForward);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_MOVE_BACKWARD, this, &ASpectator::MoveForward);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_STRAFE_LEFT, this, &ASpectator::MoveRight);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_STRAFE_RIGHT, this, &ASpectator::MoveRight);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_JUMP, this, &ASpectator::MoveUp);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_CROUCH, this, &ASpectator::MoveUp);

	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_TURN_RIGHT, this, &ASpectator::AddControllerYawInput);
	PlayerInputComponent->BindAxis(ABasePlayerController::ACTION_LOOK_UP, this, &ASpectator::AddControllerPitchInput);

	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_SPRINT, IE_Pressed, this, &ASpectator::Sprint);
	PlayerInputComponent->BindAction(ABasePlayerController::ACTION_SPRINT, IE_Released, this, &ASpectator::StopSprinting);
}

void ASpectator::AddControllerYawInput(float Val)
{
	Val *= GetController<ABasePlayerController>()->GetHorizontalSensitivity();
	Super::AddControllerYawInput(Val);
}

void ASpectator::AddControllerPitchInput(float Val)
{
	Val *= GetController<ABasePlayerController>()->GetVerticalSensitivity();
	Super::AddControllerPitchInput(Val);
}

void ASpectator::UpdateMovingMode()
{
	if (bSprintActionPressed)
	{
		FloatingPawnMovement->MaxSpeed = SprintingSpeed;
	}
	else
	{
		FloatingPawnMovement->MaxSpeed = RunningSpeed;
	}
}

void ASpectator::MoveForward(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator ViewRotation(Rotation.Pitch, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(ViewRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASpectator::MoveRight(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASpectator::MoveUp(float Value)
{
	if (Controller && (Value != 0.0f))
	{
		const FVector Direction = FRotationMatrix(FRotator(0, 0, 0)).GetUnitAxis(EAxis::Z);
		AddMovementInput(Direction, Value);
	}
}

void ASpectator::Sprint()
{
	bSprintActionPressed = true;
	UpdateMovingMode();
}

void ASpectator::StopSprinting()
{
	bSprintActionPressed = false;
	UpdateMovingMode();
}

