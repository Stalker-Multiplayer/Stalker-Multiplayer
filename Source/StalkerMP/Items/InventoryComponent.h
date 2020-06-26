// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InventoryComponent.generated.h"

class APlayerCharacter;
class USphereComponent;
class UInventoryUI;
class ABaseItem;
class ABaseStackableItem;
class AArmorItem;
class ABaseWeaponItem;
class ADeadBodyBackpack;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUpDelegate, ABaseItem*, PickedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDroppedDelegate, ABaseItem*, DroppedItem);

USTRUCT()
struct FBackpackItemWrapper
{
	GENERATED_BODY()

public:
	UPROPERTY()
		ABaseItem* Item = NULL;

	UPROPERTY()
		FIntPoint InventoryPosition = FIntPoint(-1, -1);


	bool operator==(const FBackpackItemWrapper OtherItem) const
	{
		return Item == OtherItem.Item;
	}

	bool operator!=(const FBackpackItemWrapper OtherItem) const
	{
		return Item != OtherItem.Item;
	}

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERMP_API UInventoryComponent : public USceneComponent
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Delegates --------------------------------------------------------------------------

public:

	static FOnItemPickedUpDelegate OnItemPickedUpDelegate;
	static FOnItemDroppedDelegate OnItemDroppedDelegate;




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	virtual void BeginPlay();




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		APlayerCharacter* PlayerCharacter;

	UPROPERTY()
		USphereComponent* PickupSphere;

	UPROPERTY()
		UInventoryUI* InventoryUI;

	UPROPERTY()
		TArray<ABaseItem*> ItemsAvailableForPickup;

	UPROPERTY(Replicated)
		TArray<ABaseItem*> AllItems;

	UPROPERTY(ReplicatedUsing = OnRep_ItemsInBackpack)
		TArray<FBackpackItemWrapper> ItemsInBackpack;

	UPROPERTY(ReplicatedUsing = OnRep_ArmorItem)
		AArmorItem* ArmorItem;

	UPROPERTY(Replicated)
		ABaseWeaponItem* WeaponItem;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponSlot1Item)
		ABaseWeaponItem* WeaponSlot1Item;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponSlot2Item)
		ABaseWeaponItem* WeaponSlot2Item;

	UPROPERTY(Replicated)
		ABaseWeaponItem* WeaponSlot3Item;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<ABaseWeaponItem> WeaponSlot3ItemClass;

	UPROPERTY()
		FBackpackItemWrapper EMPTY_BACKPACK_ITEM_WRAPPER = FBackpackItemWrapper();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FIntPoint BackpackSize = FIntPoint(1, 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* PickupSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* DropSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* ItemMoveSound;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:


	// General

	UFUNCTION()
		void AddItemIfNeeded(ABaseItem* Item);

	UFUNCTION()
		void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	// Utils

	UFUNCTION()
		bool RectanglesOverlap(FIntPoint L1, FIntPoint R1, FIntPoint L2, FIntPoint R2)
		{
			if (L1.X > R2.X || L2.X > R1.X)
			{
				return false;
			}

			if (L1.Y > R2.Y || L2.Y > R1.Y)
			{
				return false;
			}

			return true;
		}

	UFUNCTION()
		FIntPoint InvertPoint(FIntPoint Point)
		{
			return FIntPoint(Point.Y, Point.X);
		}


	// Backpack

	UFUNCTION()
		FBackpackItemWrapper Backpack_GetItemWrapper(ABaseItem* Item);

	UFUNCTION()
		FIntPoint Backpack_FindPlaceToPutItemTo(ABaseItem* Item);


	// Items

	UFUNCTION()
		void RemoveFromPreviousSlot(ABaseItem* Item, bool UpdateOnlySlot);

	UFUNCTION()
		void SetWeaponItem(ABaseWeaponItem* TheWeaponItem);

	UFUNCTION()
		void OnRep_ItemsInBackpack();

	UFUNCTION()
		void OnRep_ArmorItem();

	UFUNCTION()
		void OnRep_WeaponSlot1Item();

	UFUNCTION()
		void OnRep_WeaponSlot2Item();


public:


	// General

	UFUNCTION()
		bool CanPickupItem(ABaseItem* Item);

	UFUNCTION()
		void FillDeadBodyBackpack(ADeadBodyBackpack* DeadBodyBackpack);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_PickupItem(ABaseItem* Item);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_DropItem(ABaseItem* Item);
	
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_DropAllItems();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_RemoveFromInventory(ABaseItem* Item);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_DestroyItem(ABaseItem* Item);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_OnItemUsed(ABaseItem* Item);


	// Backpack

	UFUNCTION()
		FBackpackItemWrapper Backpack_GetItemAt(int X, int Y);

	UFUNCTION()
		bool Backpack_CanPutItemTo(ABaseItem* Item, int X, int Y);

	UFUNCTION()
		bool Backpack_CanAddToExistingStacks(ABaseStackableItem* StackableItem);

	UFUNCTION()
		bool Backpack_CanAddToStackAt(ABaseStackableItem* StackableItem, int X, int Y);

	UFUNCTION(Server, Reliable, WithValidation)
		void Backpack_Server_PutItemTo(ABaseItem* Item, int X, int Y);

	UFUNCTION(Server, Reliable, WithValidation)
		void Backpack_Server_AddToStackAt(ABaseStackableItem* StackableItem, int X, int Y);

	UFUNCTION(Server, Reliable, WithValidation)
		void Backpack_Server_AddToExistingStacks(ABaseStackableItem* StackabeItem);

	UFUNCTION(Server, Reliable, WithValidation)
		void Backpack_Server_RemoveFromExistingStacks(TSubclassOf<ABaseStackableItem> StackableItemClass, int RemoveAmount);

	UFUNCTION()
		int Backpack_CountItemsOfType(TSubclassOf<ABaseItem> ItemType);

	UFUNCTION()
		int Backpack_CountStackableItemsOfType(TSubclassOf<ABaseStackableItem> ItemType);

	UFUNCTION()
		void Backpack_GetItemsOfType(TSubclassOf<ABaseItem> ItemType, TArray<ABaseItem*> &ReturnArray);

	UFUNCTION()
		void Backpack_GetStackableItemsOfType(TSubclassOf<ABaseStackableItem> ItemType, TArray<ABaseStackableItem*> &ReturnArray);


	// Slots

	UFUNCTION(Server, Reliable, WithValidation)
		void Armor_Server_PutItem(AArmorItem* TheArmorItem);

	UFUNCTION(Server, Reliable, WithValidation)
		void Weapon1_Server_PutItem(ABaseWeaponItem* TheWeaponItem);

	UFUNCTION(Server, Reliable, WithValidation)
		void Weapon2_Server_PutItem(ABaseWeaponItem* TheWeaponItem);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_UseWeapon(ABaseWeaponItem* TheWeaponItem);

	UFUNCTION()
		ABaseWeaponItem* GetWeapon1();

	UFUNCTION()
		ABaseWeaponItem* GetWeapon2();

	UFUNCTION()
		ABaseWeaponItem* GetWeapon3();


	// Etc

	UFUNCTION()
		void OnPossess();

	UFUNCTION()
		void SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter);

	UFUNCTION()
		void SetPickupSphere(USphereComponent* ThePickupSphere);

	UFUNCTION()
		void SetInventoryUI(UInventoryUI* TheInventoryUI);

	UFUNCTION()
		FIntPoint GetBackpackSize() { return BackpackSize; }

	UFUNCTION()
		void OnItemUpdate(ABaseItem* Item);

	UFUNCTION(Client, Reliable)
		void Client_OnItemPickedUp(ABaseItem* Item);

	UFUNCTION(Client, Reliable)
		void Client_OnItemDropped(ABaseItem* Item);
	
	UFUNCTION()
		// We need this this keep items replication relevant when owner moves
		void UpdateItemsLocation(FVector Location);

	UFUNCTION(Client, Reliable, BlueprintCallable)
		void Client_PlaySound(USoundBase* SoundToPlay);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void Multicast_PlaySound(USoundBase* SoundToPlay);

	UFUNCTION()
		void DestroyAllItems();


};
