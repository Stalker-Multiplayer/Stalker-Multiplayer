// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStaticMeshItem.h"
#include "ArmorItem.generated.h"

class USkeletalMesh;

/**
 * 
 */
UCLASS()
class STALKERMP_API AArmorItem : public ABaseStaticMeshItem
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	virtual void FillItemDetails() override;


public:
	// Sets default values for this actor's properties
	AArmorItem();




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USkeletalMesh* CharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USkeletalMesh* FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float BulletDamageReduction = 0.99;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool IncludesHelmet = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int FallVelocityReduction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool CanRun = true;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION(BlueprintCallable)
		USkeletalMesh* GetCharacterMesh() { return CharacterMesh; }

	UFUNCTION(BlueprintCallable)
		USkeletalMesh* GetFirstPersonMesh() { return FirstPersonMesh; }

	UFUNCTION(BlueprintPure)
		float GetBulletDamageReduction(EBodyPart BodyPart);

	UFUNCTION(BlueprintPure)
		int GetFallVelocityReduction() { return FallVelocityReduction; }

	UFUNCTION(BlueprintPure)
		bool GetCanRun() { return CanRun; }

	
	
};
