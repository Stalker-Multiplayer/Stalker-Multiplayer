// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseSkeletalMeshItem.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseSkeletalMeshItem : public ABaseItem
{
	GENERATED_BODY()
	
	

// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	ABaseSkeletalMeshItem();

	void OnRep_CanBePickedUp() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:


	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* PickupCollision;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		USkeletalMeshComponent* GetMesh() { return SkeletalMeshComponent; }

	
};
