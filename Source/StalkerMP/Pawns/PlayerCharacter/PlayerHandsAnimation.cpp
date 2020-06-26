// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHandsAnimation.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Items/BaseWeaponItem.h"
#include "StalkerMP/Items/FirearmWeaponItem.h"


void UPlayerHandsAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		DrawAnimationRate = DrawAnimationLength / PlayerCharacter->GetWeaponItem()->GetDrawTime();
		HolsterAnimationRate = HolsterAnimationLength / PlayerCharacter->GetWeaponItem()->GetHolsterTime();

		if (AFirearmWeaponItem* FirearmWeaponItem = dynamic_cast<AFirearmWeaponItem*>(PlayerCharacter->GetWeaponItem())) {
			ReloadAnimationRate = ReloadAnimationLength / FirearmWeaponItem->GetReloadTime();
		}
	}
}

void UPlayerHandsAnimation::NativeUpdateAnimation(float DeltaTimeX)
{
	if (PlayerCharacter)
	{
		UpdateMovingMode();
		UpdateSpeed();
		UpdateWeaponHoldState();
	}
}

void UPlayerHandsAnimation::UpdateMovingMode()
{
	MovingMode = PlayerCharacter->GetMovingMode();
}

void UPlayerHandsAnimation::UpdateSpeed()
{
	Speed = PlayerCharacter->GetHorizontalVelocity().Size();
}

void UPlayerHandsAnimation::UpdateWeaponHoldState()
{
	WeaponHoldState = PlayerCharacter->GetWeaponHoldState();
}

void UPlayerHandsAnimation::PlayAttackAnimation(bool PrimaryAnimation)
{
	if (PrimaryAnimation)
	{
		FireMontage = PlaySlotAnimationAsDynamicMontage(FireAnimation, FName("DefaultSlot"), 0.1, 0.1);
	}
	else
	{
		FireMontage = PlaySlotAnimationAsDynamicMontage(SecondaryFireAnimation, FName("DefaultSlot"), 0.1, 0.1);
	}
}

void UPlayerHandsAnimation::StopAttackAnimation()
{
	if (FireMontage)
	{
		Montage_Stop(0.1, FireMontage);
		FireMontage = nullptr;
	}
}

void UPlayerHandsAnimation::OnWeaponDrawFinished()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->OnWeaponDrawFinished();
	}
}

void UPlayerHandsAnimation::OnWeaponHolster()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->OnWeaponHolsterFinished();
	}
}