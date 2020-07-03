// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterAnimation.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerCharacterAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerCharacterAnimation::NativeUpdateAnimation(float DeltaTimeX)
{
	if (PlayerCharacter)
	{
		UpdateMovingMode();
		UpdateSpeed();
		UpdateRotation();
		UpdateCrouching();
		UpdateInAir();
		UpdateAimRotation();
		UpdateLeanRotation();
		UpdateWeaponHoldState();
		UpdateIsDoingLongAction();
	}
}

void UPlayerCharacterAnimation::OnStepLeft()
{
	if (!PlayerCharacter->HasAuthority() && !PlayerCharacter->IsFalling())
	{
		FVector FootLocation = GetLeftFootLocation();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GetStepSound(), FootLocation, FRotator(0, 0, 0), PlayerCharacter->GetMovingSpeedScale());
	}
}

void UPlayerCharacterAnimation::OnStepRight()
{
	if (!PlayerCharacter->HasAuthority() && !PlayerCharacter->IsFalling())
	{
		FVector FootLocation = GetRightFootLocation();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GetStepSound(), FootLocation, FRotator(0, 0, 0), PlayerCharacter->GetMovingSpeedScale());
	}
}

void UPlayerCharacterAnimation::OnLanded()
{
	if (!PlayerCharacter->HasAuthority())
	{
		FVector FootLocation = GetLeftFootLocation();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GetStepSound(), FootLocation);
		FootLocation = GetRightFootLocation();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GetStepSound(), FootLocation);
	}
}

USoundBase* UPlayerCharacterAnimation::GetStepSound()
{
	bool IsOnGround = false;
	EPhysicalSurface PhysicalSurface = GetSurfaceType(IsOnGround);
	if (IsOnGround)
	{
		USoundBase** StepSound = Map_StepSounds.Find(PhysicalSurface);
		if (!StepSound || !*StepSound)
		{
			return FallbackStepSound;
		}
		return *StepSound;
	}
	else
	{
		return nullptr;
	}
}

FVector UPlayerCharacterAnimation::GetLeftFootLocation()
{
	return PlayerCharacter->GetMesh()->GetSocketLocation(LeftFootSocketName);
}

FVector UPlayerCharacterAnimation::GetRightFootLocation()
{
	return PlayerCharacter->GetMesh()->GetSocketLocation(RightFootSocketName);
}

void UPlayerCharacterAnimation::UpdateMovingMode()
{
	MovingMode = PlayerCharacter->GetMovingMode();
}

void UPlayerCharacterAnimation::UpdateSpeed()
{
	Speed = PlayerCharacter->GetHorizontalVelocity().Size();
}

void UPlayerCharacterAnimation::UpdateRotation()
{
	const FVector HorizontalVelocity = PlayerCharacter->GetHorizontalVelocity();
	const FRotator Rotator = PlayerCharacter->GetActorRotation();
	Rotation = CalculateDirection(HorizontalVelocity, Rotator);
}

void UPlayerCharacterAnimation::UpdateCrouching()
{
	IsCrouching = PlayerCharacter->bIsCrouched;
}

void UPlayerCharacterAnimation::UpdateInAir()
{
	IsInAir = PlayerCharacter->IsFalling();
	PlayerJumped = PlayerCharacter->bWasJumping;
}

void UPlayerCharacterAnimation::UpdateAimRotation()
{
	float Roll = PlayerCharacter->GetLookRotator().Pitch;

	if (Roll > 180)
	{
		AimRotation.Roll = (360 - Roll);
	}
	else
	{
		AimRotation.Roll = -1 * Roll;
	}
}

void UPlayerCharacterAnimation::UpdateLeanRotation()
{
	float RollRad = UKismetMathLibrary::DegreesToRadians(PlayerCharacter->GetLookRotator().Pitch);
	LeanRotation.Pitch = PlayerCharacter->GetLeanAngle() * UKismetMathLibrary::Cos(RollRad);
	LeanRotation.Yaw = PlayerCharacter->GetLeanAngle() * -UKismetMathLibrary::Sin(RollRad);
}

void UPlayerCharacterAnimation::UpdateWeaponHoldState()
{
	WeaponHoldState = PlayerCharacter->GetWeaponHoldState();
}

void UPlayerCharacterAnimation::UpdateIsDoingLongAction()
{
	IsDoingLongAction = PlayerCharacter->IsDoingLongAction();
}

void UPlayerCharacterAnimation::SetWeaponTorso(ETorsoAnimation TorsoAnimation, float ReloadTime, float DrawTime, float HolsterTime)
{
	CurrentTorsoAnimation = TorsoAnimation;
	ReloadAnimationRate = Map_ReloadAnimationLength[TorsoAnimation] / ReloadTime;
	DrawAnimationRate = Map_DrawAnimationLength[TorsoAnimation] / DrawTime;
	DrawCrouchedAnimationRate = Map_DrawCrouchedAnimationLength[TorsoAnimation] / DrawTime;
	HolsterAnimationRate = Map_HolsterAnimationLength[TorsoAnimation] / HolsterTime;
	HolsterCrouchedAnimationRate = Map_HolsterCrouchedAnimationLength[TorsoAnimation] / HolsterTime;
}

void UPlayerCharacterAnimation::PlayAttackAnimation(bool PrimaryAnimation)
{
	if (PrimaryAnimation)
	{
		if (Map_PrimaryFireAnimations.Contains(CurrentTorsoAnimation))
		{
			StandingFireMontage = PlayAttackAnimationForSlot(Map_PrimaryFireAnimations[CurrentTorsoAnimation].Standing, FName("StandingFireSlot"));
			StandingAimingFireMontage = PlayAttackAnimationForSlot(Map_PrimaryFireAnimations[CurrentTorsoAnimation].StandingAiming, FName("StandingAimingFireSlot"));
			CrouchingFireMontage = PlayAttackAnimationForSlot(Map_PrimaryFireAnimations[CurrentTorsoAnimation].Crouching, FName("CrouchingFireSlot"));
			CrouchingAimingFireMontage = PlayAttackAnimationForSlot(Map_PrimaryFireAnimations[CurrentTorsoAnimation].CrouchingAiming, FName("CrouchingAimingFireSlot"));
		}
	}
	else
	{
		if (Map_SecondaryFireAnimations.Contains(CurrentTorsoAnimation))
		{
			StandingFireMontage = PlayAttackAnimationForSlot(Map_SecondaryFireAnimations[CurrentTorsoAnimation].Standing, FName("StandingFireSlot"));
			StandingAimingFireMontage = PlayAttackAnimationForSlot(Map_SecondaryFireAnimations[CurrentTorsoAnimation].StandingAiming, FName("StandingAimingFireSlot"));
			CrouchingFireMontage = PlayAttackAnimationForSlot(Map_SecondaryFireAnimations[CurrentTorsoAnimation].Crouching, FName("CrouchingFireSlot"));
			CrouchingAimingFireMontage = PlayAttackAnimationForSlot(Map_SecondaryFireAnimations[CurrentTorsoAnimation].CrouchingAiming, FName("CrouchingAimingFireSlot"));
		}
	}
}

void UPlayerCharacterAnimation::StopAttackAnimation()
{
	if (StandingFireMontage)
	{
		Montage_Stop(0.1, StandingFireMontage);
		StandingFireMontage = nullptr;
	}
	if (StandingAimingFireMontage)
	{
		Montage_Stop(0.1, StandingAimingFireMontage);
		StandingAimingFireMontage = nullptr;
	}
	if (CrouchingFireMontage)
	{
		Montage_Stop(0.1, CrouchingFireMontage);
		CrouchingFireMontage = nullptr;
	}
	if (CrouchingAimingFireMontage)
	{
		Montage_Stop(0.1, CrouchingAimingFireMontage);
		CrouchingAimingFireMontage = nullptr;
	}
}

UAnimMontage* UPlayerCharacterAnimation::PlayAttackAnimationForSlot(UAnimSequenceBase* Asset, FName SlotNodeName)
{
	if (Asset && CurrentSkeleton->IsCompatible(Asset->GetSkeleton()))
	{
		// create asset using the information
		UAnimMontage* NewMontage = UAnimMontage::CreateSlotAnimationAsDynamicMontage(Asset, SlotNodeName, 0.1, 0.1, 1, 1, -1, 0);

		if (NewMontage)
		{
			// if playing is successful, return the montage to allow more control if needed
			float PlayTime = Montage_Play(NewMontage, 1, EMontagePlayReturnType::MontageLength, 0, false);
			return PlayTime > 0.0f ? NewMontage : nullptr;
		}
	}

	return nullptr;
}
