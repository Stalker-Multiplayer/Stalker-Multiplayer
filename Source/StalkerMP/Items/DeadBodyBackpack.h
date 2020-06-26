// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadBodyBackpack.generated.h"

class ABaseItem;
class UStaticMeshComponent;
class UBoxComponent;
class UInventoryUI;

USTRUCT()
struct FDeadBodyBackpackItemWrapper
{
	GENERATED_BODY()

public:

	UPROPERTY()
		ABaseItem* Item = NULL;

	UPROPERTY()
		FIntPoint InventoryPosition = FIntPoint(-1, -1);

	bool operator==(const FDeadBodyBackpackItemWrapper OtherItem) const
	{
		return Item == OtherItem.Item;
	}

	bool operator!=(const FDeadBodyBackpackItemWrapper OtherItem) const
	{
		return Item != OtherItem.Item;
	}

};


UCLASS()
class STALKERMP_API ADeadBodyBackpack : public AActor
{
	GENERATED_BODY()
	



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	ADeadBodyBackpack();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UInventoryUI* InventoryUI;

	UPROPERTY(Replicated)
		TArray<ABaseItem*> AllItems;

	UPROPERTY(Replicated)
		TArray<FDeadBodyBackpackItemWrapper> ItemsInBackpack;

	UPROPERTY(Replicated)
		FIntPoint BackpackSize = FIntPoint(1, 1);

	UPROPERTY(Replicated)
		ABaseItem* ArmorItem;

	UPROPERTY(Replicated)
		ABaseItem* WeaponSlot1Item;

	UPROPERTY(Replicated)
		ABaseItem* WeaponSlot2Item;


protected:

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* ActionCollision;







// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		void SetInventoryUI(UInventoryUI* TheInventoryUI);

	UFUNCTION()
		void GetAllItems(TArray<ABaseItem*> &ArrayToFill);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_RemoveItem(ABaseItem* Item);

	UFUNCTION()
		void AddItem(ABaseItem* Item);

	UFUNCTION()
		void SetBackpackSize(FIntPoint TheBackpackSize);

	UFUNCTION()
		void AddItemToBackpack(ABaseItem* Item, FIntPoint ItemPosition);

	UFUNCTION()
		FIntPoint GetBackpackSize();

	UFUNCTION()
		void GetItemsInBackpack(TArray<FDeadBodyBackpackItemWrapper> &ArrayToFill);

	UFUNCTION()
		void SetArmorItem(ABaseItem* TheArmorItem);

	UFUNCTION()
		ABaseItem* GetArmorItem();

	UFUNCTION()
		void SetWeaponSlot1Item(ABaseItem* TheWeaponSlot1Item);

	UFUNCTION()
		ABaseItem* GetWeaponSlot1Item();

	UFUNCTION()
		void SetWeaponSlot2Item(ABaseItem* TheWeaponSlot2Item);

	UFUNCTION()
		ABaseItem* GetWeaponSlot2Item();

};
