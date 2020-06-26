// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimInstance.h"
#include "WeaponFPPAnimation.generated.h"

class ABaseWeaponItem;

/**
 * 
 */
UCLASS()
class STALKERMP_API UWeaponFPPAnimation : public UAnimInstance
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void NativeInitializeAnimation() override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY()
		UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimSequenceBase* FireAnimation;

	UPROPERTY(BlueprintReadOnly)
		ABaseWeaponItem* Weapon;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION(BlueprintCallable)
		void PlayAttackAnimation();

	UFUNCTION(BlueprintCallable)
		void StopAttackAnimation();


};