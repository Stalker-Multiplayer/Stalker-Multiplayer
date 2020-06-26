// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "Animation/AnimInstance.h"
#include "PlayerHandsAnimation.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS()
class STALKERMP_API UPlayerHandsAnimation : public UAnimInstance
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UAnimMontage* FireMontage;


protected:

	UPROPERTY()
		APlayerCharacter* PlayerCharacter;


	// Movement

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		EMovingMode MovingMode;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float Speed;


	// Etc

	UPROPERTY(BlueprintReadOnly, Category = "Etc")
		EWeaponHoldState WeaponHoldState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimSequenceBase* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimSequenceBase* SecondaryFireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ReloadAnimationLength = 1.0;

	UPROPERTY(BlueprintReadOnly)
		float ReloadAnimationRate = 1.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float DrawAnimationLength = 1.0;

	UPROPERTY(BlueprintReadOnly)
		float DrawAnimationRate = 1.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float HolsterAnimationLength = 1.0;

	UPROPERTY(BlueprintReadOnly)
		float HolsterAnimationRate = 1.0;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION()
		void UpdateMovingMode();

	UFUNCTION()
		void UpdateSpeed();

	UFUNCTION()
		void UpdateWeaponHoldState();

	UFUNCTION(BlueprintCallable)
		void OnWeaponDrawFinished();

	UFUNCTION(BlueprintCallable)
		void OnWeaponHolster();


public:

	UFUNCTION(BlueprintCallable)
		void PlayAttackAnimation(bool PrimaryAnimation);

	UFUNCTION(BlueprintCallable)
		void StopAttackAnimation();


};