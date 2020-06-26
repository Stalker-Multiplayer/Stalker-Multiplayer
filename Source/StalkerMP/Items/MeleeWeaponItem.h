// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Items/BaseWeaponItem.h"

#include "MeleeWeaponItem.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class STALKERMP_API AMeleeWeaponItem : public ABaseWeaponItem
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

	virtual void FireOnce() override;



	
// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		FTimerHandle HitDetectionTimerHandle;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int HitDistance = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int SideLinetraceCountHalf = 7;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int HitAngle = 20;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		FHitResult SwipeForHit();



protected:

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnHitEffects(const FHitResult &SweepResult);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SpawnHitEffects(const FHitResult &SweepResult);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_SpawnHitEffects(const FHitResult &SweepResult);


};
