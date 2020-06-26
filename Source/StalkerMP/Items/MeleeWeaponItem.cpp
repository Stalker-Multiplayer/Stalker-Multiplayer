// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeaponItem.h"

#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Public/TimerManager.h"


void AMeleeWeaponItem::FireOnce()
{
	if (!IsFiring || !CanFire())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
		IsInDelay = false;
		IsFiring = false;
		return;
	}

	FHitResult HitResult = SwipeForHit();

	CreateAttackEffects(true);
	TArray<int> attackIds;
	currentAttackId++;
	attackIds.Add(currentAttackId);
	Server_OnAttack(TArray<FTransform>(), attackIds);

	if (HitResult.Component.IsValid())
	{
		SpawnHitEffects(HitResult);
		Server_SpawnHitEffects(HitResult);
		if (APlayerCharacter* AnotherPlayerCharacter = dynamic_cast<APlayerCharacter*>(HitResult.Actor.Get())) {
			OnHit(HitResult, 1, currentAttackId);
		}
	}
}

FHitResult AMeleeWeaponItem::SwipeForHit()
{
	TArray<FHitResult> TempHitResults;
	FHitResult HitResult;

	for (int i = 0; i <= SideLinetraceCountHalf; i++)
	{
		TempHitResults = PlayerCharacter->LineTraceMulti(AttackCollisionChannel, HitDistance, HitAngle * 1.0f / SideLinetraceCountHalf * i, true, false);

		for (FHitResult TempHitResult : TempHitResults)
		{
			if (TempHitResult.Component.IsValid() && TempHitResult.Actor.Get() != PlayerCharacter)
			{
				HitResult = TempHitResult;
				break;
			}
		}

		if (HitResult.Component.IsValid())
		{
			break;
		}

		TempHitResults = PlayerCharacter->LineTraceMulti(AttackCollisionChannel, HitDistance, HitAngle * -1.0f / SideLinetraceCountHalf * i, true, false);

		for (FHitResult TempHitResult : TempHitResults)
		{
			if (TempHitResult.Component.IsValid() && TempHitResult.Actor.Get() != PlayerCharacter)
			{
				HitResult = TempHitResult;
				break;
			}
		}

		if (HitResult.Component.IsValid())
		{
			break;
		}
	}

	return HitResult;
}

void AMeleeWeaponItem::Server_SpawnHitEffects_Implementation(const FHitResult &SweepResult)
{
	Multicast_SpawnHitEffects(SweepResult);
}

bool AMeleeWeaponItem::Server_SpawnHitEffects_Validate(const FHitResult &SweepResult)
{
	return true;
}

void AMeleeWeaponItem::Multicast_SpawnHitEffects_Implementation(const FHitResult &SweepResult)
{
	if (!HasAuthority() && PlayerCharacter && !PlayerCharacter->IsLocallyControlled()) {
		SpawnHitEffects(SweepResult);
	}
}
