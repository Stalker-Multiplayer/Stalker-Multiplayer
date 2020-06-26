// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"
#include "StalkerMP/Items/WeaponFPPAnimation.h"

#include "FirearmWeaponFPPAnimation.generated.h"

class AFirearmWeaponItem;

/**
 * 
 */
UCLASS()
class STALKERMP_API UFirearmWeaponFPPAnimation : public UWeaponFPPAnimation
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(BlueprintReadOnly)
		AFirearmWeaponItem* FirearmWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Etc")
		bool IsReloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ReloadAnimationLength = 1.0;

	UPROPERTY(BlueprintReadOnly)
		float ReloadAnimationRate = 1.0;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void UpdateIsReloading();


protected:

	UFUNCTION(BlueprintCallable)
		void PlayReloadSound();



};