// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Items/BaseStackableItem.h"

#include "MedicineItem.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API AMedicineItem : public ABaseStackableItem
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	virtual void FillItemDetails() override;
	virtual void BeforeUse() override;
	virtual void AfterUse() override;


public:
	// Sets default values for this actor's properties
	AMedicineItem();

	void Server_Use_Implementation() override;





// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float HealAmount;


};
