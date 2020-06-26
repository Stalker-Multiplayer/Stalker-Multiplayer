// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "Animation/AnimInstance.h"
#include "WeaponTPPAnimation.generated.h"

class ABaseWeaponItem;

/**
 * 
 */
UCLASS()
class STALKERMP_API UWeaponTPPAnimation : public UAnimInstance
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

	UPROPERTY(BlueprintReadOnly)
		ABaseWeaponItem* Weapon;

	UPROPERTY(BlueprintReadOnly)
		EWeaponAnimState AnimState = EWeaponAnimState::OnGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimSequenceBase* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FVector DefaultTranslation = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FRotator DefaultRotation = FRotator(90, 90, 0);

	UPROPERTY(BlueprintReadOnly)
		FVector CurrentTranslation = DefaultTranslation;

	UPROPERTY(BlueprintReadOnly)
		FRotator CurrentRotation = DefaultRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FVector HoldTranslation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FRotator HoldRotation;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void UpdateAnimState();


public:

	UFUNCTION(BlueprintCallable)
		void PlayAttackAnimation();

	UFUNCTION(BlueprintCallable)
		void StopAttackAnimation();


};