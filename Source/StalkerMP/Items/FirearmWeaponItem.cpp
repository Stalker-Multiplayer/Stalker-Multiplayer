// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeaponItem.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerHandsAnimation.h"
#include "StalkerMP/Items/BaseStackableItem.h"
#include "StalkerMP/Items/InventoryComponent.h"
#include "StalkerMP/Items/FirearmWeaponFPPAnimation.h"
#include "StalkerMP/Items/WeaponTPPAnimation.h"

#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/TimerManager.h"
#include "Net/UnrealNetwork.h"


const int AFirearmWeaponItem::LINE_TRACE_LENGTH = 15000;
const int AFirearmWeaponItem::RECOIL_RECOVER = 5;
const int AFirearmWeaponItem::CAMERA_ROLL_SCALE = 25;
const float AFirearmWeaponItem::MIN_INACCURACY_TO_RESET = 0.05;
const float AFirearmWeaponItem::INACCURACY_UPDATE_SCALE = 0.1;


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

AFirearmWeaponItem::AFirearmWeaponItem()
{
	FireRoundModes.Add(-1);
}

void AFirearmWeaponItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFirearmWeaponItem, HasScope);
	DOREPLIFETIME(AFirearmWeaponItem, HasSilencer);
	DOREPLIFETIME(AFirearmWeaponItem, HasLauncher);
	DOREPLIFETIME(AFirearmWeaponItem, IsReloading);
}

void AFirearmWeaponItem::BeginPlay()
{
	if (!HasAuthority())
	{
		ABaseItem* AmmoItem = GetWorld()->SpawnActor<ABaseItem>(AmmoItemClass.Get());
		AmmoName = AmmoItem->GetItemName();
		AmmoItem->Destroy();
	}

	Super::BeginPlay();

	FireRoundMode = FireRoundModes[0];

	MaxBulletSpreadRad = UKismetMathLibrary::Acos((20000 - HundredMetersBulletSpread * HundredMetersBulletSpread) / 20000);
	StandingInaccuracyRad = UKismetMathLibrary::DegreesToRadians(StandingInaccuracyAngle);
	RunningInaccuracyRad = UKismetMathLibrary::DegreesToRadians(RunningInaccuracyAngle);

	OnRep_HasScope();
	OnRep_HasSilencer();
	OnRep_HasLauncher();
}

void AFirearmWeaponItem::Tick(float DeltaSeconds)
{
	if (PlayerController && PlayerCharacter && !HasAuthority())
	{
		UpdateRecoil(DeltaSeconds);
		UpdateInaccuracy(DeltaSeconds);
	}

	Super::Tick(DeltaSeconds);
}

void AFirearmWeaponItem::FillItemDetails()
{
	Super::FillItemDetails();

	FFormatNamedArguments Args;

	Args.Add("BulletSpeed", BulletSpeed / 100);
	Args.Add("MaxDistance", BulletSpeed / 100 * BulletLifeSpanSec);
	Args.Add("SpreadAt100", HundredMetersBulletSpread);

	Args.Add("ShotsPerMinute", ShotsPerMinute);
	Args.Add("ShotsPerSecond", ShotsPerMinute / 60);

	Args.Add("VerticalRecoil", VerticalRecoilStrength * 10);
	Args.Add("AimVerticalRecoil", AimVerticalRecoilStrength * 10);
	Args.Add("HorizontalRecoil", HorizontalRecoilStrength * 10);

	Args.Add("StandingInaccuracy", StandingInaccuracyAngle);
	Args.Add("RunningInaccuracy", StandingInaccuracyAngle + RunningInaccuracyAngle);
	Args.Add("CrouchInaccuracy", StandingInaccuracyAngle * CrouchInaccuracyScale);

	Args.Add("AmmoName", AmmoName);

	ItemDetails = FText::Format(ItemDetails, Args);
}

void AFirearmWeaponItem::SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter)
{
	Super::SetPlayerCharacter(ThePlayerCharacter);

	if (ThePlayerCharacter) {
		Client_UpdateAmmo(Ammo);
	}
}

void AFirearmWeaponItem::OnPossess()
{
	Client_UpdateAmmo(Ammo);
}


// ------------------------------------------------------------------------- Parts --------------------------------------------------------------------------

void AFirearmWeaponItem::OnRep_HasScope()
{
	if (HasScope)
	{
		SkeletalMeshComponent->UnHideBoneByName(ScopeSocketName);
		FPPMeshComponent->UnHideBoneByName(ScopeSocketName);
	}
	else
	{
		SkeletalMeshComponent->HideBoneByName(ScopeSocketName, PBO_None);
		FPPMeshComponent->HideBoneByName(ScopeSocketName, PBO_None);
	}
}

void AFirearmWeaponItem::OnRep_HasSilencer()
{
	if (HasSilencer)
	{
		SkeletalMeshComponent->UnHideBoneByName(SilencerSocketName);
		FPPMeshComponent->UnHideBoneByName(SilencerSocketName);
	}
	else
	{
		SkeletalMeshComponent->HideBoneByName(SilencerSocketName, PBO_None);
		FPPMeshComponent->HideBoneByName(SilencerSocketName, PBO_None);
	}
}

void AFirearmWeaponItem::OnRep_HasLauncher()
{
	if (HasLauncher)
	{
		SkeletalMeshComponent->UnHideBoneByName(LauncherSocketName);
		FPPMeshComponent->UnHideBoneByName(LauncherSocketName);
	}
	else
	{
		SkeletalMeshComponent->HideBoneByName(LauncherSocketName, PBO_None);
		FPPMeshComponent->HideBoneByName(LauncherSocketName, PBO_None);
	}
}

FName AFirearmWeaponItem::GetFireEffectSocketName()
{
	if (HasSilencer)
	{
		return MuzzleSilencerSocketName;
	}
	else
	{
		return MuzzleSocketName;
	}
}

FVector AFirearmWeaponItem::GetMuzzleLocation()
{
	if (PlayerCharacter->IsLocallyControlled())
	{
		return FPPMeshComponent->GetSocketLocation(GetFireEffectSocketName());
	}
	else
	{
		return SkeletalMeshComponent->GetSocketLocation(GetFireEffectSocketName());
	}
}


// ------------------------------------------------------------------------- Firing --------------------------------------------------------------------------

bool AFirearmWeaponItem::CanFire()
{
	return
		Super::CanFire()
		&& Ammo > 0
		&& !IsReloading;
}

void AFirearmWeaponItem::StartFiring()
{
	if (ShootsLeftInFireRound > 0)
	{
		return;
	}

	if (!CanFire())
	{
		if (Ammo <= 0)
		{
			UGameplayStatics::SpawnSoundAttached(EmptySound, FPPMeshComponent);
		}
		return;
	}

	IsFiring = true;

	if (!IsInDelay)
	{
		ShootsLeftInFireRound = FireRoundMode;
		IsInDelay = true;
		GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &ABaseWeaponItem::FireOnce, ShootDelay, true, 0.0);
	}
}

void AFirearmWeaponItem::StopFiring()
{
	if (ShootsLeftInFireRound <= 0)
	{
		IsFiring = false;
	}
}

void AFirearmWeaponItem::SwitchFireRoundMode()
{
	if (IsFiring)
	{
		return;
	}

	for (int i = 0; i < FireRoundModes.Num(); i++)
	{
		if (FireRoundModes[i] == FireRoundMode)
		{
			i++;

			if (i >= FireRoundModes.Num())
			{
				i = 0;
			}

			FireRoundMode = FireRoundModes[i];

			break;
		}
	}
}

void AFirearmWeaponItem::FireOnce()
{
	if (!IsFiring || !CanFire() || ShootsLeftInFireRound == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
		IsInDelay = false;
		IsFiring = false;
		ShootsLeftInFireRound = 0;
		return;
	}

	if (ShootsLeftInFireRound > 0)
	{
		ShootsLeftInFireRound--;
	}

	CreateProjectiles();
	AddRecoil();
	CreateAttackEffects(true);

	Ammo--;
}

void AFirearmWeaponItem::CreateProjectiles()
{
	FHitResult HitResult = PlayerCharacter->LineTraceSingle(AttackCollisionChannel, LINE_TRACE_LENGTH, 0, false, false);
	FVector LineTraceEnd;
	if (HitResult.bBlockingHit)
	{
		LineTraceEnd = HitResult.Location;
	}
	else
	{
		LineTraceEnd = HitResult.TraceEnd;
	}

	FVector MuzzleLocation = GetMuzzleLocation();
	FRotator ProjectileLook = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, LineTraceEnd);
	FTransform BaseTransform = FTransform(ProjectileLook, MuzzleLocation, FVector(1, 1, 1));

	float Spread;
	float SpreadAngle;
	float VerticalSpread;
	float HorizontalSpread;

	float MovingSpread = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::RandomFloatInRange(0, CurrentInaccuracyRad));
	float MovingSpreadAngle = UKismetMathLibrary::DegreesToRadians(UKismetMathLibrary::RandomFloatInRange(0, 360));
	float MovingVerticalSpread = UKismetMathLibrary::Sin(MovingSpreadAngle) * MovingSpread;
	float MovingHorizontalSpread = UKismetMathLibrary::Cos(MovingSpreadAngle) * MovingSpread;
	FRotator MovingAimRotator = FRotator(MovingHorizontalSpread, MovingVerticalSpread, 0);

	TArray<FTransform> SpawnTransforms;
	TArray<int> attackIds;

	for (int i = 0; i < SingleShotBulletAmount; i++)
	{
		// Calculating bullet spread
		FTransform ProjectileTransform = BaseTransform;
		Spread = UKismetMathLibrary::RadiansToDegrees(UKismetMathLibrary::RandomFloatInRange(0, MaxBulletSpreadRad));
		SpreadAngle = UKismetMathLibrary::DegreesToRadians(UKismetMathLibrary::RandomFloatInRange(0, 360));
		VerticalSpread = UKismetMathLibrary::Sin(SpreadAngle) * Spread;
		HorizontalSpread = UKismetMathLibrary::Cos(SpreadAngle) * Spread;
		ProjectileTransform.SetRotation((ProjectileTransform.Rotator() + FRotator(HorizontalSpread, VerticalSpread, 0) + MovingAimRotator).Quaternion());

		currentAttackId++;
		attackIds.Add(currentAttackId);
		CreateProjectile(ProjectileTransform, currentAttackId);

		SpawnTransforms.Add(ProjectileTransform);
	}

	Server_OnAttack(SpawnTransforms, attackIds);
}

void AFirearmWeaponItem::AddRecoil()
{
	CurrentRecoilScale = 1;

	CurrentHorizontalRecoil += UKismetMathLibrary::RandomFloatInRange(-HorizontalRecoilStrength, HorizontalRecoilStrength);
	if (CurrentHorizontalRecoil < -HorizontalRecoilStrength)
	{
		CurrentHorizontalRecoil = -HorizontalRecoilStrength;
	}
	else if (CurrentHorizontalRecoil > HorizontalRecoilStrength)
	{
		CurrentHorizontalRecoil = HorizontalRecoilStrength;
	}

	CurrentVerticalRecoil +=
		(PlayerCharacter->GetMovingMode() != EMovingMode::Aimimg ? -VerticalRecoilStrength : -AimVerticalRecoilStrength)
		* (PlayerCharacter->bIsCrouched ? CrouchInaccuracyScale : 1);

	if (PlayerCharacter->GetMovingMode() != EMovingMode::Aimimg)
	{
		CurrentInaccuracyRad += HorizontalRecoilStrength;
	}
}

void AFirearmWeaponItem::UpdateRecoil(float DeltaSeconds)
{
	float LeanAngleRad = UKismetMathLibrary::DegreesToRadians(PlayerCharacter->GetLeanAngle());

	CurrentVerticalRecoil *= CurrentRecoilScale;
	PlayerController->AddPitchInput(CurrentVerticalRecoil * UKismetMathLibrary::Cos(LeanAngleRad)
		+ CurrentHorizontalRecoil * -UKismetMathLibrary::Sin(LeanAngleRad));

	CurrentHorizontalRecoil *= CurrentRecoilScale;
	PlayerController->AddYawInput(CurrentVerticalRecoil * -UKismetMathLibrary::Sin(LeanAngleRad)
		+ CurrentHorizontalRecoil * UKismetMathLibrary::Cos(LeanAngleRad));

	FRotator ControlRotation = PlayerController->GetControlRotation();
	ControlRotation.Roll = CurrentHorizontalRecoil * CAMERA_ROLL_SCALE;
	PlayerController->SetControlRotation(ControlRotation);

	CurrentRecoilScale -= RECOIL_RECOVER * DeltaSeconds / Handling;
	if (CurrentRecoilScale < 0)
	{
		CurrentRecoilScale = 0;
	}
}

void AFirearmWeaponItem::UpdateInaccuracy(float DeltaSeconds)
{
	if (PlayerCharacter->GetMovingMode() != EMovingMode::Aimimg)
	{
		TargetInaccuracyRad = StandingInaccuracyRad
			* ((PlayerCharacter->bIsCrouched) ? CrouchInaccuracyScale : 1);
		TargetInaccuracyRad += RunningInaccuracyRad
			* ((PlayerCharacter->bIsCrouched) ? CrouchInaccuracyScale : 1)
			* PlayerCharacter->GetMovingSpeedScale();
	}
	else
	{
		TargetInaccuracyRad = 0;
	}

	float Change = (TargetInaccuracyRad - CurrentInaccuracyRad) / (INACCURACY_UPDATE_SCALE / Handling) * DeltaSeconds;
	if (UKismetMathLibrary::Abs(Change) < UKismetMathLibrary::Abs(CurrentInaccuracyRad - TargetInaccuracyRad))
	{
		CurrentInaccuracyRad += Change;
	}
	else
	{
		CurrentInaccuracyRad = TargetInaccuracyRad;
	}
}

void AFirearmWeaponItem::Server_OnAttack_Implementation(const TArray<FTransform>& SpawnTransforms, const TArray<int>& attackIds)
{
	Ammo--;

	if (Ammo >= 0) {
		server_attackIds.Append(attackIds);
		Multicast_OnAttack(SpawnTransforms);
	}
}

void AFirearmWeaponItem::Multicast_OnAttack_Implementation(const TArray<FTransform>& SpawnTransforms)
{
#if !UE_SERVER
	if (!HasAuthority() && PlayerCharacter && !PlayerCharacter->IsLocallyControlled())
	{
		CreateAttackEffects(true);

		for (FTransform ProjectileTransform : SpawnTransforms)
		{
			CreateProjectile(ProjectileTransform, 0);
		}
	}
#endif
}

void AFirearmWeaponItem::OnProjectileHit(float LifeSpan, const FHitResult& SweepResult, int attackId)
{
	if (PlayerCharacter->IsLocallyControlled())
	{
		OnHit(SweepResult, LifeSpan / BulletLifeSpanSec, attackId);
	}
}

void AFirearmWeaponItem::OnRep_IsReloading()
{
	if (IsReloading)
	{
		if (TPPAnimation)
		{
			TPPAnimation->StopAttackAnimation();
			PlayerCharacter->StopAttackAnimation();
		}
	}
	else
	{
		if (ReloadAudioComponent)
		{
			ReloadAudioComponent->Stop();
		}
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	}

	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsReloading(IsReloading);
	}
}

void AFirearmWeaponItem::Reload()
{
	if (!IsReloading)
	{
		if (Ammo < MaxAmmo && InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass) > 0)
		{
			IsReloading = true;
			PlayerCharacter->SetIsReloading(true);
			AnimState = EWeaponAnimState::Reloading;
			((UFirearmWeaponFPPAnimation*) FPPAnimation)->StopAttackAnimation();
			HandsAnimation->StopAttackAnimation();
			PlayerCharacter->StopAttackAnimation();
			Server_StartReload();

			if (ReloadType == EReloadType::Magazine)
			{
				OnReloadStep();
			}
			else if (ReloadType == EReloadType::Single)
			{
				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFirearmWeaponItem::OnReloadStep, ReloadTime, false, ReloadTime);
			}
		}
	}
}

void AFirearmWeaponItem::Server_StartReload_Implementation()
{
	if (Ammo < MaxAmmo && InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass) > 0)
	{
		IsReloading = true;
		PlayerCharacter->SetIsReloading(true);
		AnimState = EWeaponAnimState::Reloading;

		if (ReloadType == EReloadType::Magazine)
		{
			OnReloadStep();
		}
		else if (ReloadType == EReloadType::Single)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFirearmWeaponItem::OnReloadStep, ReloadTime, false, ReloadTime);
		}
	}
}

bool AFirearmWeaponItem::Server_StartReload_Validate()
{
	return true;
}

void AFirearmWeaponItem::OnReloadStep()
{
	if (Ammo < MaxAmmo && InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass) > 0)
	{
		AnimState = EWeaponAnimState::Reloading;
		if (ReloadType == EReloadType::Magazine)
		{
			GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFirearmWeaponItem::FinishReload, ReloadTime, false, ReloadTime);
		}
		else if (ReloadType == EReloadType::Single)
		{
			if (Ammo < MaxAmmo && InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass) > 0)
			{
				if (HasAuthority())
				{
					Ammo++;
					InventoryComponent->Backpack_Server_RemoveFromExistingStacks(AmmoItemClass, 1);
					Client_UpdateAmmo(Ammo);

					if (Ammo < MaxAmmo && InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass) > 0)
					{
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFirearmWeaponItem::OnReloadStep, ReloadTime, false, ReloadTime);
					}
					else
					{
						FinishReload();
					}
				}
				else
				{
					if (Ammo + 1 < MaxAmmo && InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass) - 1 > 0)
					{
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFirearmWeaponItem::OnReloadStep, ReloadTime, false, ReloadTime);
					}
					else
					{
						FinishReload();
					}
				}
			}
		}
	}
}

void AFirearmWeaponItem::FinishReload()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	AnimState = EWeaponAnimState::Hold;

	if (HasAuthority())
	{
		if (ReloadType == EReloadType::Magazine)
		{
			int AmmoToAdd = InventoryComponent->Backpack_CountStackableItemsOfType(AmmoItemClass);
			if (AmmoToAdd > MaxAmmo - Ammo)
			{
				AmmoToAdd = MaxAmmo - Ammo;
			}
			Ammo += AmmoToAdd;
			InventoryComponent->Backpack_Server_RemoveFromExistingStacks(AmmoItemClass, AmmoToAdd);

			Client_UpdateAmmo(Ammo);
		}
	}

	IsReloading = false;
	PlayerCharacter->SetIsReloading(false);
}

void AFirearmWeaponItem::CancelReload()
{
	if (IsReloading)
	{
		if (HasAuthority())
		{
			Client_CancelReload();
			Server_CancelReload_Implementation();
		}
		else
		{
			Client_CancelReload_Implementation();
			Server_CancelReload();
		}
	}
}

void AFirearmWeaponItem::Client_CancelReload_Implementation()
{
	if (ReloadAudioComponent)
	{
		ReloadAudioComponent->Stop();
	}
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	IsReloading = false;
	PlayerCharacter->SetIsReloading(false);
	Server_CancelReload();
}

void AFirearmWeaponItem::Server_CancelReload_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	IsReloading = false;
	PlayerCharacter->SetIsReloading(false);
}

bool AFirearmWeaponItem::Server_CancelReload_Validate()
{
	return true;
}

void AFirearmWeaponItem::Client_UpdateAmmo_Implementation(int NewAmmo)
{
	Ammo = NewAmmo;
}

float AFirearmWeaponItem::GetInaccuracyRad()
{
	return CurrentInaccuracyRad + MaxBulletSpreadRad;
}

void AFirearmWeaponItem::SetAmmo(int NewAmmo)
{
	Ammo = NewAmmo;
	Client_UpdateAmmo(NewAmmo);
}

// ------------------------------------------------------------------------- Effects --------------------------------------------------------------------------

void AFirearmWeaponItem::PlayReloadSound()
{
	if (PlayerCharacter->IsLocallyControlled())
	{
		ReloadAudioComponent = UGameplayStatics::SpawnSoundAttached(ReloadSound, FPPMeshComponent);
	}
	else
	{
		ReloadAudioComponent = UGameplayStatics::SpawnSoundAttached(ReloadSound, SkeletalMeshComponent);
	}
}
