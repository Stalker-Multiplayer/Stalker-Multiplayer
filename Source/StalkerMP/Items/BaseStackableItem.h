// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/Items/BaseStaticMeshItem.h"

#include "BaseStackableItem.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseStackableItem : public ABaseStaticMeshItem
{
	GENERATED_BODY()
	
	


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:

	virtual void FillItemDetails() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		int StackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int MaxStackSize = 1;





// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_ForceUpdateStackSize(int TheStackSize);

	UFUNCTION(BlueprintCallable)
		int AddToStack(int AddAmount);

	UFUNCTION(BlueprintCallable)
		int RemoveFromStack(int RemoveAmount);

	UFUNCTION(BlueprintCallable)
		bool IsStackEmpty() { return StackSize <= 0; }

	UFUNCTION(BlueprintCallable)
		int GetFreeStackSize() { return MaxStackSize - StackSize; }

	UFUNCTION(BlueprintCallable)
		int GetStackSize() { return StackSize; }

	UFUNCTION(BlueprintCallable)
		int GetMaxStackSize() { return MaxStackSize; }


};
