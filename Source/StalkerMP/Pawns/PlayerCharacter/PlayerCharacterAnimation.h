// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimation.generated.h"

class APlayerCharacter;

class USoundBase;

USTRUCT(BlueprintType)
struct FFireAnimationStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
		UAnimSequenceBase* Standing;

	UPROPERTY(EditDefaultsOnly)
		UAnimSequenceBase* StandingAiming;

	UPROPERTY(EditDefaultsOnly)
		UAnimSequenceBase* Crouching;

	UPROPERTY(EditDefaultsOnly)
		UAnimSequenceBase* CrouchingAiming;

};

/**
 * 
 */
UCLASS()
class STALKERMP_API UPlayerCharacterAnimation : public UAnimInstance
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UAnimMontage* StandingFireMontage;

	UPROPERTY()
		UAnimMontage* StandingAimingFireMontage;

	UPROPERTY()
		UAnimMontage* CrouchingFireMontage;

	UPROPERTY()
		UAnimMontage* CrouchingAimingFireMontage;


protected:

	UPROPERTY()
		APlayerCharacter* PlayerCharacter;


	// Movement

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		EMovingMode MovingMode;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float Rotation;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool IsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool IsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool PlayerJumped;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool IsDoingLongAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		USoundBase* FallbackStepSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		TMap<TEnumAsByte<EPhysicalSurface>, USoundBase*> Map_StepSounds;


	// Animation

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		FRotator AimRotation = FRotator();

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		ETorsoAnimation CurrentTorsoAnimation = ETorsoAnimation::Torso0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, FFireAnimationStruct> Map_PrimaryFireAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, FFireAnimationStruct> Map_SecondaryFireAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, float> Map_ReloadAnimationLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, float> Map_DrawAnimationLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, float> Map_DrawCrouchedAnimationLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, float> Map_HolsterAnimationLength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		TMap<ETorsoAnimation, float> Map_HolsterCrouchedAnimationLength;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float ReloadAnimationRate = 1.0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float DrawAnimationRate = 1.0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float DrawCrouchedAnimationRate = 1.0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float HolsterAnimationRate = 1.0;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		float HolsterCrouchedAnimationRate = 1.0;


	// Etc

	UPROPERTY(BlueprintReadOnly, Category = "Etc")
		EWeaponHoldState WeaponHoldState;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		UAnimMontage* PlayAttackAnimationForSlot(UAnimSequenceBase* Asset, FName SlotNodeName);


protected:

	UFUNCTION()
		USoundBase* GetStepSound();

	UFUNCTION(BlueprintPure, BlueprintCosmetic)
		FVector GetStepSoundLocation();

	UFUNCTION(BlueprintCallable)
		void OnStep();

	UFUNCTION(BlueprintImplementableEvent)
		EPhysicalSurface GetSurfaceType(bool& IsOnGround);

	UFUNCTION()
		void UpdateMovingMode();

	UFUNCTION()
		void UpdateSpeed();

	UFUNCTION()
		void UpdateRotation();

	UFUNCTION()
		void UpdateCrouching();

	UFUNCTION()
		void UpdateInAir();

	UFUNCTION()
		void UpdateAimRotation();

	UFUNCTION()
		void UpdateWeaponHoldState();

	UFUNCTION()
		void UpdateIsDoingLongAction();


public:

	UFUNCTION(BlueprintCallable)
		void OnLanded();

	UFUNCTION()
		void SetWeaponTorso(ETorsoAnimation TorsoAnimation, float ReloadTime, float DrawTime, float HolsterTime);

	UFUNCTION(BlueprintCallable)
		void PlayAttackAnimation(bool PrimaryAnimation);

	UFUNCTION(BlueprintCallable)
		void StopAttackAnimation();

};