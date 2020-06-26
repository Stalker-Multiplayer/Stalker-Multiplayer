// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponFPPAnimation.h"

#include "StalkerMP/Items/BaseWeaponItem.h"

#include "Animation/AnimMontage.h"


void UWeaponFPPAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Weapon = Cast<ABaseWeaponItem>(GetOwningActor());
}

void UWeaponFPPAnimation::PlayAttackAnimation()
{
	FireMontage = PlaySlotAnimationAsDynamicMontage(FireAnimation, FName("DefaultSlot"), 0, 0);
}

void UWeaponFPPAnimation::StopAttackAnimation()
{
	if (FireMontage)
	{
		Montage_Stop(0, FireMontage);
		FireMontage = nullptr;
	}
}