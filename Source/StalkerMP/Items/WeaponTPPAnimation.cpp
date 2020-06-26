// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponTPPAnimation.h"

#include "StalkerMP/Items/BaseWeaponItem.h"

#include "Animation/AnimMontage.h"


void UWeaponTPPAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Weapon = Cast<ABaseWeaponItem>(GetOwningActor());
}

void UWeaponTPPAnimation::NativeUpdateAnimation(float DeltaTimeX)
{
	if (Weapon)
	{
		UpdateAnimState();
	}
}

void UWeaponTPPAnimation::UpdateAnimState()
{
	AnimState = Weapon->GetAnimState();

	switch (AnimState)
	{
	case EWeaponAnimState::OnGround:
		CurrentTranslation = DefaultTranslation;
		CurrentRotation = DefaultRotation;
		break;
	case EWeaponAnimState::Hold:
	case EWeaponAnimState::Reloading:
		CurrentTranslation = HoldTranslation;
		CurrentRotation = HoldRotation + DefaultRotation;
		break;
	}
}

void UWeaponTPPAnimation::PlayAttackAnimation()
{
	FireMontage = PlaySlotAnimationAsDynamicMontage(FireAnimation, FName("DefaultSlot"), 0, 0);
}

void UWeaponTPPAnimation::StopAttackAnimation()
{
	if (FireMontage)
	{
		Montage_Stop(0, FireMontage);
		FireMontage = nullptr;
	}
}
