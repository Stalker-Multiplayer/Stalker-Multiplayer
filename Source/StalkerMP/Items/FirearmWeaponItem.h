// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Items/BaseWeaponItem.h"

#include "FirearmWeaponItem.generated.h"

class ABaseStackableItem;

class UAudioComponent;
class USoundBase;

/**
 * 
 */
UCLASS()
class STALKERMP_API AFirearmWeaponItem : public ABaseWeaponItem
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:

	static const int LINE_TRACE_LENGTH;
	static const int RECOIL_RECOVER;
	static const int CAMERA_ROLL_SCALE;
	static const float MIN_INACCURACY_TO_RESET;
	static const float INACCURACY_UPDATE_SCALE;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

	AFirearmWeaponItem();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void FillItemDetails() override;
	virtual void SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter) override;
	virtual void OnPossess() override;

	virtual bool CanFire() override;
	virtual void StartFiring() override;
	virtual void StopFiring() override;
	virtual void FireOnce() override;
	virtual FName GetFireEffectSocketName() override;

	virtual void Server_OnAttack_Implementation(const TArray<FTransform>& SpawnTransforms, const TArray<int>& attackIds);
	virtual void Multicast_OnAttack_Implementation(const TArray<FTransform>& SpawnTransforms);



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	// Firing

	UPROPERTY()
		int FireRoundMode;

	UPROPERTY()
		int ShootsLeftInFireRound = 0;

	UPROPERTY()
		float CurrentRecoilScale = 0;

	UPROPERTY()
		float CurrentVerticalRecoil = 0;

	UPROPERTY()
		float CurrentHorizontalRecoil = 0;

	UPROPERTY()
		float MaxBulletSpreadRad = 0.01; // 1 meter spread at 100 meters distance

	UPROPERTY()
		float StandingInaccuracyRad = 0.25;

	UPROPERTY()
		float RunningInaccuracyRad = 1;

	UPROPERTY()
		float TargetInaccuracyRad = 1;

	UPROPERTY()
		float CurrentInaccuracyRad = 0;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;

	UPROPERTY()
		FTimerHandle ReloadSoundTimerHandle;

	UPROPERTY()
		UAudioComponent* ReloadAudioComponent;

	UPROPERTY()
		FText AmmoName;



protected:

	// Parts

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ScopeSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName SilencerSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName LauncherSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName MuzzleSilencerSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName MuzzleSocketName;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_HasScope)
		bool HasScope = false;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_HasSilencer)
		bool HasSilencer = false;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_HasLauncher)
		bool HasLauncher = false;


	// Firing

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsReloading)
		bool IsReloading = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int MaxAmmo = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Firing")
		int Ammo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int BulletSpeed = 5000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		float BulletLifeSpanSec = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		int SingleShotBulletAmount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		float ReloadTime = 1.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
		TArray<int> FireRoundModes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EReloadType ReloadType = EReloadType::Magazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float HundredMetersBulletSpread = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float StandingInaccuracyAngle = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float RunningInaccuracyAngle = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float CrouchInaccuracyScale = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float VerticalRecoilStrength = -1.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float AimVerticalRecoilStrength = -2.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Accuracy")
		float HorizontalRecoilStrength = 0.3;


	// Effects

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		USoundBase* ReloadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		USoundBase* EmptySound;



public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullets")
		TSubclassOf<ABaseStackableItem> AmmoItemClass;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	// Parts

	UFUNCTION()
		void OnRep_HasScope();

	UFUNCTION()
		void OnRep_HasSilencer();

	UFUNCTION()
		void OnRep_HasLauncher();


	// Firing

	UFUNCTION()
		void CreateProjectiles();

	UFUNCTION()
		void AddRecoil();

	UFUNCTION()
		void UpdateRecoil(float DeltaSeconds);

	UFUNCTION()
		void UpdateInaccuracy(float DeltaSeconds);

	UFUNCTION(Client, Reliable)
		void Client_UpdateAmmo(int NewAmmo);


	// Etc

	UFUNCTION()
		void OnRep_IsReloading();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_StartReload();

	UFUNCTION()
		void OnReloadStep();

	UFUNCTION(Client, Reliable)
		void Client_CancelReload();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_CancelReload();

	UFUNCTION()
		void FinishReload();



protected:


	// Parts

	UFUNCTION()
		FVector GetMuzzleLocation();


	// Firing

	UFUNCTION(BlueprintImplementableEvent)
		void CreateProjectile(FTransform Transform, int attackId);



public:

	// Firing

	UFUNCTION(BlueprintCallable)
		void SwitchFireRoundMode();

	UFUNCTION()
		int GetFireRoundMode() { return FireRoundMode; }

	UFUNCTION()
		float GetInaccuracyRad();

	UFUNCTION()
		void Reload();

	UFUNCTION()
		void CancelReload();

	UFUNCTION()
		bool GetIsReloading() { return IsReloading; }

	UFUNCTION()
		float GetReloadTime() { return ReloadTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetMaxAmmo() { return MaxAmmo; }

	UFUNCTION(BlueprintCallable)
		void SetAmmo(int NewAmmo);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetAmmo() { return Ammo; }

	UFUNCTION()
		void PlayReloadSound();

	UFUNCTION(BlueprintCallable)
		void OnProjectileHit(float LifeSpan, const FHitResult& SweepResult, int attackId);


};
