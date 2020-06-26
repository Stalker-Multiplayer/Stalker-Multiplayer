// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "BaseSkeletalMeshItem.h"
#include "BaseWeaponItem.generated.h"

class ABasePlayerController;
class UWeaponFPPAnimation;
class UWeaponTPPAnimation;
class UPlayerHandsAnimation;

class UAnimInstance;
class UParticleSystem;
class USoundBase;
class UPhysicalMaterial;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseWeaponItem : public ABaseSkeletalMeshItem
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const float DRAW_TIME_SCALE;
	static const float HOLSTER_TIME_SCALE;




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	ABaseWeaponItem();

	virtual void BeginPlay() override;

	virtual void FillItemDetails() override;
	virtual void SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		float DrawTime = 1;

	UPROPERTY()
		float HolsterTime = 1;


protected:

	UPROPERTY()
		UWeaponFPPAnimation* FPPAnimation;

	UPROPERTY()
		UWeaponTPPAnimation* TPPAnimation;

	UPROPERTY()
		UPlayerHandsAnimation* HandsAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* FPPMeshComponent;

	UPROPERTY(BlueprintReadOnly, Replicated)
		ABasePlayerController* PlayerController;


	// Parts

	UFUNCTION()
		virtual FName GetFireEffectSocketName();


	// Firing

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TEnumAsByte<ECollisionChannel> AttackCollisionChannel = ECC_Visibility;

	UPROPERTY()
		bool IsInDelay = false;

	UPROPERTY()
		float ShootDelay = 1.0;

	UPROPERTY(BlueprintReadOnly)
		bool IsFiring = false;

	UPROPERTY()
		FTimerHandle ShootTimerHandle;

	UPROPERTY(BlueprintReadOnly)
		EWeaponAnimState AnimState = EWeaponAnimState::OnGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int ShotsPerMinute = 60;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int BaseDamage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		float HeadDamageScale = 2.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		float ArmDamageScale = 0.4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		float LegDamageScale = 0.7;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Handling = 1;

	UPROPERTY()
		int currentAttackId = 0;

	UPROPERTY()
		TArray<int> server_attackIds;


	// Effects

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* SecondaryFireEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		USoundBase* SecondaryFireSound;


	// Etc

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ETorsoAnimation TorsoAnimation = ETorsoAnimation::Torso0;


public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UAnimInstance> HandsAnimationClass = nullptr;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:


	// Firing

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_OnAttack(const TArray<FTransform>& SpawnTransforms, const TArray<int>& attackIds);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_OnAttack(const TArray<FTransform>& SpawnTransforms);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_OnHit(const FHitResult& SweepResult, float DamageScale, int attackId);

	UFUNCTION()
		void OnHit(const FHitResult& SweepResult, float DamageScale, int attackId);


	// Effects

	UFUNCTION()
		void CreateAttackEffects(bool PrimaryAttack);


public:

	UFUNCTION()
		USkeletalMeshComponent* GetFPPMesh() { return FPPMeshComponent; }


	// Firing

	UFUNCTION(BlueprintCallable)
		virtual void StartFiring();

	UFUNCTION(BlueprintCallable)
		virtual void StopFiring();

	UFUNCTION()
		virtual void FireOnce();

	UFUNCTION()
		virtual bool CanFire();


	// Etc

	UFUNCTION()
		void SetAnimState(EWeaponAnimState TheAnimState);

	UFUNCTION()
		void SetHandsAnimation(UPlayerHandsAnimation* TheHandsAnimation) { HandsAnimation = TheHandsAnimation; }

	UFUNCTION()
		ETorsoAnimation GetTorsoAnimation() { return TorsoAnimation; }

	UFUNCTION()
		EWeaponAnimState GetAnimState() { return AnimState; }

	UFUNCTION()
		float GetDrawTime() { return DrawTime; }

	UFUNCTION()
		float GetHolsterTime() { return HolsterTime; }

};
