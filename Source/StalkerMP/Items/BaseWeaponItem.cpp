// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeaponItem.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerHandsAnimation.h"
#include "StalkerMP/Items/ArmorItem.h"
#include "StalkerMP/Items/InventoryComponent.h"
#include "StalkerMP/Items/WeaponFPPAnimation.h"
#include "StalkerMP/Items/WeaponTPPAnimation.h"
#include "StalkerMP/DamageTypes/BulletDamageType.h"

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/TimerManager.h"
#include "Net/UnrealNetwork.h"



const float ABaseWeaponItem::DRAW_TIME_SCALE = 0.75;
const float ABaseWeaponItem::HOLSTER_TIME_SCALE = 0.5;


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

ABaseWeaponItem::ABaseWeaponItem()
{
	CanBeUsed = true;

	PrimaryActorTick.bCanEverTick = true;

	FPPMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPPMeshComponent"));
	FPPMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	FPPMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	FPPMeshComponent->bRenderStatic = true;
	FPPMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	FPPMeshComponent->bComponentUseFixedSkelBounds = true;
	FPPMeshComponent->CanCharacterStepUpOn = ECB_No;
	FPPMeshComponent->SetGenerateOverlapEvents(false);
	FPPMeshComponent->SetCanEverAffectNavigation(false);
	FPPMeshComponent->CastShadow = true;
	FPPMeshComponent->bSelfShadowOnly = true;
	FPPMeshComponent->bPerBoneMotionBlur = false;
	FPPMeshComponent->SetVisibility(false);
	FPPMeshComponent->SetupAttachment(RootComponent);
}

void ABaseWeaponItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABaseWeaponItem, PlayerController, COND_OwnerOnly);
}

void ABaseWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		FPPAnimation = (UWeaponFPPAnimation*)FPPMeshComponent->GetAnimInstance();
		TPPAnimation = (UWeaponTPPAnimation*)SkeletalMeshComponent->GetAnimInstance();
	}

	ShootDelay = 60.0 / ShotsPerMinute;
	DrawTime = DRAW_TIME_SCALE / Handling;
	HolsterTime = HOLSTER_TIME_SCALE / Handling;
}

void ABaseWeaponItem::FillItemDetails()
{
	Super::FillItemDetails();

	FFormatNamedArguments Args;

	Args.Add("HeadDamage", BaseDamage * HeadDamageScale);
	Args.Add("BodyDamage", BaseDamage);
	Args.Add("ArmDamage", BaseDamage * ArmDamageScale);
	Args.Add("LegDamage", BaseDamage * LegDamageScale);

	Args.Add("Handling", Handling);

	ItemDetails = FText::Format(ItemDetails, Args);
}

void ABaseWeaponItem::SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter)
{
	Super::SetPlayerCharacter(ThePlayerCharacter);

	if (ThePlayerCharacter) {
		if (HasAuthority() || ThePlayerCharacter->IsLocallyControlled())
		{
			PlayerController = ThePlayerCharacter->GetController<ABasePlayerController>();
		}
		else
		{
			PlayerController = nullptr;
		}
	}
	else
	{
		PlayerController = nullptr;
		IsFiring = false;
		GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
	}
}




// ------------------------------------------------------------------------- Parts --------------------------------------------------------------------------


FName ABaseWeaponItem::GetFireEffectSocketName()
{
	return FName("");
}

void ABaseWeaponItem::SetAnimState(EWeaponAnimState TheAnimState)
{
	AnimState = TheAnimState;
}




// ------------------------------------------------------------------------- Firing --------------------------------------------------------------------------

bool ABaseWeaponItem::CanFire()
{
	return PlayerCharacter->GetMovingMode() != EMovingMode::Sprinting;
}

void ABaseWeaponItem::StartFiring()
{
	IsFiring = true;

	if (!IsInDelay)
	{
		IsInDelay = true;
		GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &ABaseWeaponItem::FireOnce, ShootDelay, true, 0.0);
	}
}

void ABaseWeaponItem::StopFiring()
{
	IsFiring = false;
}

void ABaseWeaponItem::FireOnce()
{
	if (!IsFiring || !CanFire())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
		IsInDelay = false;
		IsFiring = false;
		return;
	}

	CreateAttackEffects(true);
	TArray<int> attackIds;
	currentAttackId++;
	attackIds.Add(currentAttackId);
	Server_OnAttack(TArray<FTransform>(), attackIds);
}

void ABaseWeaponItem::OnHit(const FHitResult& SweepResult, float DamageScale, int attackId)
{
	if (PlayerCharacter->IsLocallyControlled())
	{
		Server_OnHit(SweepResult, DamageScale, attackId);
	}
}

void ABaseWeaponItem::Server_OnAttack_Implementation(const TArray<FTransform>& SpawnTransforms, const TArray<int>& attackIds)
{
	server_attackIds.Append(attackIds);
	Multicast_OnAttack(SpawnTransforms);
}

bool ABaseWeaponItem::Server_OnAttack_Validate(const TArray<FTransform>& SpawnTransforms, const TArray<int>& attackIds)
{
	return true;
}

void ABaseWeaponItem::Multicast_OnAttack_Implementation(const TArray<FTransform>& SpawnTransforms)
{
#if !UE_SERVER
	if (!HasAuthority() && !PlayerCharacter->IsLocallyControlled())
	{
		CreateAttackEffects(true);
	}
#endif
}

void ABaseWeaponItem::Server_OnHit_Implementation(const FHitResult& SweepResult, float DamageScale, int attackId)
{
	if (server_attackIds.Contains(attackId))
	{
		server_attackIds.Remove(attackId);

		if (APlayerCharacter* DamagedCharacter = dynamic_cast<APlayerCharacter*>(SweepResult.GetActor()))
		{
			EBodyPart BodyPart = DamagedCharacter->BoneToBodyPart(SweepResult.BoneName);

			float BodyPartScale = 1;
			switch (BodyPart)
			{
			case EBodyPart::Head:
				BodyPartScale = HeadDamageScale;
				break;
			case EBodyPart::Arm:
				BodyPartScale = ArmDamageScale;
				break;
			case EBodyPart::Leg:
				BodyPartScale = LegDamageScale;
				break;
			}

			float ArmorScale;
			if (DamagedCharacter->GetArmorItem())
			{
				ArmorScale = (1 - DamagedCharacter->GetArmorItem()->GetBulletDamageReduction(BodyPart));
			}
			else
			{
				ArmorScale = 1;
			}

			DamagedCharacter->TakeDamage(BaseDamage * BodyPartScale * ArmorScale * DamageScale, FDamageEvent(UBulletDamageType::StaticClass()), PlayerCharacter->GetController(), PlayerCharacter);
		}
	}
}

bool ABaseWeaponItem::Server_OnHit_Validate(const FHitResult& SweepResult, float DamageScale, int attackId)
{
	return true;
}



// ------------------------------------------------------------------------- Effects --------------------------------------------------------------------------

void ABaseWeaponItem::CreateAttackEffects(bool PrimaryAttack)
{
	if (PlayerCharacter->IsLocallyControlled())
	{
		UGameplayStatics::SpawnEmitterAttached(FireEffect, FPPMeshComponent, GetFireEffectSocketName(),
			FVector(0, 0, 0), FRotator(0, 0, 0), FVector(1, 1, 1), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None);
		UGameplayStatics::SpawnSoundAttached(FireSound, FPPMeshComponent, GetFireEffectSocketName(),
			FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, true);
		if (TPPAnimation)
		{
			FPPAnimation->PlayAttackAnimation();
		}
		if (TPPAnimation)
		{
			HandsAnimation->PlayAttackAnimation(PrimaryAttack);
		}
		PlayerCharacter->PlayAttackAnimation(PrimaryAttack);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAttached(FireEffect, SkeletalMeshComponent, GetFireEffectSocketName(),
			FVector(0, 0, 0), FRotator(0, 0, 0), FVector(1, 1, 1), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None);
		UGameplayStatics::SpawnSoundAttached(FireSound, SkeletalMeshComponent, GetFireEffectSocketName(),
			FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset, true, 1.f, 1.f, 0.f, nullptr, nullptr, true);
		if (TPPAnimation)
		{
			TPPAnimation->PlayAttackAnimation();
		}
		PlayerCharacter->PlayAttackAnimation(PrimaryAttack);
	}
}
