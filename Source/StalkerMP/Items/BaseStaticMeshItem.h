// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BaseStaticMeshItem.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseStaticMeshItem : public ABaseItem
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	ABaseStaticMeshItem();

	void OnRep_CanBePickedUp() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:


	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* PickupCollision;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		UStaticMeshComponent* GetMesh() { return StaticMeshComponent; }


};
