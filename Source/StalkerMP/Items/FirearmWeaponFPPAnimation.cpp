// Fill out your copyright notice in the Description page of Project Settings.

#include "FirearmWeaponFPPAnimation.h"

#include "StalkerMP/Items/FirearmWeaponItem.h"


void UFirearmWeaponFPPAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Weapon)
	{
		FirearmWeapon = (AFirearmWeaponItem*)Weapon;
		ReloadAnimationRate = ReloadAnimationLength / FirearmWeapon->GetReloadTime();
	}
}

void UFirearmWeaponFPPAnimation::NativeUpdateAnimation(float DeltaTimeX)
{
	if (FirearmWeapon)
	{
		UpdateIsReloading();
	}
}

void UFirearmWeaponFPPAnimation::UpdateIsReloading()
{
	IsReloading = FirearmWeapon->GetIsReloading();
}

void UFirearmWeaponFPPAnimation::PlayReloadSound()
{
	FirearmWeapon->PlayReloadSound();
}
