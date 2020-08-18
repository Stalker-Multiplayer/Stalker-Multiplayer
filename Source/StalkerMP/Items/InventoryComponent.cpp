// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

#include "StalkerMP/Items/HUD/InventoryUI.h"
#include "StalkerMP/Items/HUD/ItemWidget.h"
#include "StalkerMP/Items/DeadBodyBackpack.h"
#include "StalkerMP/Items/BaseItem.h"
#include "StalkerMP/Items/BaseStackableItem.h"
#include "StalkerMP/Items/ArmorItem.h"
#include "StalkerMP/Items/BaseWeaponItem.h"
#include "StalkerMP/Items/FirearmWeaponItem.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"



FOnItemPickedUpDelegate UInventoryComponent::OnItemPickedUpDelegate = FOnItemPickedUpDelegate();
FOnItemDroppedDelegate UInventoryComponent::OnItemDroppedDelegate = FOnItemDroppedDelegate();

// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, AllItems, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, ItemsInBackpack, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, ArmorItem, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, WeaponItem, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, WeaponSlot1Item, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, WeaponSlot2Item, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, WeaponSlot3Item, COND_OwnerOnly);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		WeaponSlot3Item = GetWorld()->SpawnActor<ABaseWeaponItem>(WeaponSlot3ItemClass, GetComponentTransform(), SpawnInfo);
		WeaponSlot3Item->SetActorHiddenInGame(true);
		AddItemIfNeeded(WeaponSlot3Item);
	}
}




// ------------------------------------------------------------------------- General --------------------------------------------------------------------------

void UInventoryComponent::SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter)
{
	PlayerCharacter = ThePlayerCharacter;

	if (GetOwner()->HasAuthority())
	{
		for (ABaseItem* Item : AllItems)
		{
			Item->SetPlayerCharacter(PlayerCharacter);
		}
	}
}

void UInventoryComponent::AddItemIfNeeded(ABaseItem* Item)
{
	if (!AllItems.Contains(Item))
	{
		AllItems.Add(Item);
		Item->SetOwner(GetOwner());
		Item->SetInventoryComponent(this);
		Item->SetPlayerCharacter(PlayerCharacter);
		Item->SetPickable(false);
		OnItemPickedUpDelegate.Broadcast(Item);

		if (PlayerCharacter && PlayerCharacter->GetController())
		{
			PlayerCharacter->Multicast_PlaySound(PickupSound);
			Client_OnItemPickedUp(Item);
		}
	}
}

void UInventoryComponent::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (ABaseItem* Item = dynamic_cast<ABaseItem*>(OtherActor)) {
		ItemsAvailableForPickup.Add(Item);
		if (PlayerCharacter->IsLocallyControlled())
		{
			Item->CreateItemWidget(InventoryUI);
			Item->SetInventoryComponentForPickup(this);
			InventoryUI->OnPickableItemAppeared(Item->GetItemWidget());
		}
	}
}

void UInventoryComponent::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABaseItem* Item = dynamic_cast<ABaseItem*>(OtherActor)) {
		if (PlayerCharacter->IsLocallyControlled())
		{
			InventoryUI->OnPickableItemGone(Item->GetItemWidget());
			Item->SetInventoryComponentForPickup(nullptr);
			if (Item->GetOwner() != GetOwner())
			{
				Item->RemoveItemWidget();
			}
		}
		ItemsAvailableForPickup.Remove(Item);
	}
}

void UInventoryComponent::UpdateItemsLocation(FVector Location)
{
	for (ABaseItem* Item : AllItems)
	{
		if (Item)
		{
			Item->SetActorLocation(Location);
		}
	}
}

bool UInventoryComponent::CanPickupItem(ABaseItem* Item)
{
	if (AArmorItem* TheArmorItem = dynamic_cast<AArmorItem*>(Item)) {
		if (!ArmorItem)
		{
			return true;
		}
	}

	if (ABaseWeaponItem* TheWeaponItem = dynamic_cast<ABaseWeaponItem*>(Item)) {
		if (!WeaponSlot1Item)
		{
			return true;
		}
		if (!WeaponSlot2Item)
		{
			return true;
		}
	}

	if (ABaseStackableItem* TheStackableItem = dynamic_cast<ABaseStackableItem*>(Item)) {
		if (Backpack_CanAddToExistingStacks(TheStackableItem))
		{
			return true;
		}
	}

	FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(Item);

	return (PlaceToPutItemTo.X != -1 && PlaceToPutItemTo.Y != -1);
}

void UInventoryComponent::FillDeadBodyBackpack(ADeadBodyBackpack* DeadBodyBackpack)
{
	for (ABaseItem* Item : AllItems) {
		DeadBodyBackpack->AddItem(Item);
	}

	DeadBodyBackpack->SetBackpackSize(BackpackSize);
	for (FBackpackItemWrapper BackpackItemWrapper : ItemsInBackpack)
	{
		DeadBodyBackpack->AddItemToBackpack(BackpackItemWrapper.Item, BackpackItemWrapper.InventoryPosition);
		if (ABaseStackableItem* StackableItem = dynamic_cast<ABaseStackableItem*>(BackpackItemWrapper.Item)) {
			StackableItem->Multicast_ForceUpdateStackSize(StackableItem->GetStackSize());
		}
	}

	if (ArmorItem)
	{
		DeadBodyBackpack->SetArmorItem(ArmorItem);
	}
	if (WeaponSlot1Item)
	{
		DeadBodyBackpack->SetWeaponSlot1Item(WeaponSlot1Item);
	}
	if (WeaponSlot2Item)
	{
		DeadBodyBackpack->SetWeaponSlot2Item(WeaponSlot2Item);
	}
}

void UInventoryComponent::Server_PickupItem_Implementation(ABaseItem* Item)
{
	if (Item->IsActorBeingDestroyed())
	{
		return;
	}

	if (AArmorItem* TheArmorItem = dynamic_cast<AArmorItem*>(Item)) {
		if (!ArmorItem)
		{
			Armor_Server_PutItem(TheArmorItem);
			return;
		}
	}

	if (ABaseWeaponItem* TheWeaponItem = dynamic_cast<ABaseWeaponItem*>(Item)) {
		if (!WeaponSlot1Item)
		{
			Weapon1_Server_PutItem(TheWeaponItem);
			return;
		}
		if (!WeaponSlot2Item)
		{
			Weapon2_Server_PutItem(TheWeaponItem);
			return;
		}
	}

	if (ABaseStackableItem* TheStackableItem = dynamic_cast<ABaseStackableItem*>(Item)) {
		if (Backpack_CanAddToExistingStacks(TheStackableItem))
		{
			Backpack_Server_AddToExistingStacks(TheStackableItem);
			if (TheStackableItem->IsStackEmpty())
			{
				if (PlayerCharacter->GetController())
				{
					PlayerCharacter->Multicast_PlaySound(PickupSound);
				}
				return;
			}
		}
	}

	FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(Item);
	if (PlaceToPutItemTo.X != -1 && PlaceToPutItemTo.Y != -1)
	{
		Backpack_Server_PutItemTo(Item, PlaceToPutItemTo.X, PlaceToPutItemTo.Y);
	}
}

bool UInventoryComponent::Server_PickupItem_Validate(ABaseItem* Item)
{
	return true;
}

void UInventoryComponent::Server_DropItem_Implementation(ABaseItem* Item)
{
	if (!Item->GetOwner())
	{
		return;
	}

	RemoveFromPreviousSlot(Item, false);
	AllItems.Remove(Item);
	Client_OnItemDropped(Item);
	PlayerCharacter->Multicast_PlaySound(DropSound);

	FTransform ItemSpawnTransformOffset = Item->GetSpawnTransformOffset();

	FVector NewLocation = PlayerCharacter->GetActorLocation();
	NewLocation += ItemSpawnTransformOffset.GetLocation();
	NewLocation.Z -= PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FRotator NewRotation = PlayerCharacter->GetActorRotation();
	NewRotation += ItemSpawnTransformOffset.GetRotation().Rotator();
	NewRotation.Yaw += 90;

	Item->SetOwner(nullptr);
	Item->SetInventoryComponent(nullptr);
	Item->SetPlayerCharacter(nullptr);
	Item->SetActorHiddenInGame(false);
	Item->Multicast_SetPosition(NewLocation, NewRotation);
	Item->SetPickable(true);

	if (ABaseStackableItem* StackableItem = dynamic_cast<ABaseStackableItem*>(Item)) {
		StackableItem->Multicast_ForceUpdateStackSize(StackableItem->GetStackSize());
	}

	OnItemDroppedDelegate.Broadcast(Item);
}

bool UInventoryComponent::Server_DropItem_Validate(ABaseItem* Item)
{
	return true;
}


// ------------------------------------------------------------------------- Slots --------------------------------------------------------------------------

void UInventoryComponent::RemoveFromPreviousSlot(ABaseItem* Item, bool UpdateOnlySlot)
{
	if (Item->GetInventoryComponent())
	{
		if (Item->GetInventoryComponent() == this)
		{
			FBackpackItemWrapper BackpackItemWrapper = Backpack_GetItemWrapper(Item);
			if (BackpackItemWrapper == EMPTY_BACKPACK_ITEM_WRAPPER)
			{
			}
			else {
				ItemsInBackpack.Remove(BackpackItemWrapper);
			}

			if (ArmorItem == Item)
			{
				ArmorItem = NULL;
				if (!UpdateOnlySlot)
				{
					PlayerCharacter->SetArmorItem(NULL);
				}
			}
			
			if (WeaponSlot1Item == Item)
			{
				WeaponSlot1Item = NULL;
				
				if (!UpdateOnlySlot && WeaponItem == Item)
				{
					if (WeaponSlot2Item)
					{
						SetWeaponItem(WeaponSlot2Item);
					}
					else
					{
						SetWeaponItem(WeaponSlot3Item);
					}
				}
			}

			if (WeaponSlot2Item == Item)
			{
				WeaponSlot2Item = NULL;

				if (!UpdateOnlySlot && WeaponItem == Item)
				{
					if (WeaponSlot1Item)
					{
						SetWeaponItem(WeaponSlot1Item);
					}
					else
					{
						SetWeaponItem(WeaponSlot3Item);
					}
				}
			}

			if (WeaponSlot3Item == Item)
			{
				WeaponSlot3Item = NULL;

				if (!UpdateOnlySlot && WeaponItem == Item)
				{
					if (WeaponSlot1Item)
					{
						SetWeaponItem(WeaponSlot1Item);
					}
					else
					{
						SetWeaponItem(WeaponSlot2Item);
					}
				}
			}
		}
		else
		{
			if (Item->GetDeadBodyBackpack())
			{
				Item->GetDeadBodyBackpack()->Multicast_RemoveItem(Item);
			}
			Item->GetInventoryComponent()->Server_RemoveFromInventory(Item);
		}
	}
}

FBackpackItemWrapper UInventoryComponent::Backpack_GetItemWrapper(ABaseItem* Item)
{
	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item == Item)
		{
			return ItemInBackpack;
		}
	}

	return EMPTY_BACKPACK_ITEM_WRAPPER;
}

FBackpackItemWrapper UInventoryComponent::Backpack_GetItemAt(int X, int Y)
{
	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (X >= ItemInBackpack.InventoryPosition.X
			&& Y >= ItemInBackpack.InventoryPosition.Y
			&& X < ItemInBackpack.InventoryPosition.X + ItemInBackpack.Item->GetInventorySize().X
			&& Y < ItemInBackpack.InventoryPosition.Y + ItemInBackpack.Item->GetInventorySize().Y)
		{
			return ItemInBackpack;
		}
	}

	return EMPTY_BACKPACK_ITEM_WRAPPER;
}

bool UInventoryComponent::Backpack_CanAddToExistingStacks(ABaseStackableItem* StackableItem)
{
	int StackSizeLeft = StackableItem->GetStackSize();
	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ABaseStackableItem* StackableItemInBackpack = dynamic_cast<ABaseStackableItem*>(ItemInBackpack.Item)) {
			if (StackableItemInBackpack->GetClass() == StackableItem->GetClass())
			{
				if (StackableItemInBackpack->GetFreeStackSize() > 0)
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool UInventoryComponent::Backpack_CanAddToStackAt(ABaseStackableItem* StackableItem, int X, int Y)
{
	FBackpackItemWrapper ItemAtPosition = Backpack_GetItemAt(X, Y);

	if (ItemAtPosition != EMPTY_BACKPACK_ITEM_WRAPPER)
	{
		if (ABaseStackableItem* StackableItemInBackpack = dynamic_cast<ABaseStackableItem*>(ItemAtPosition.Item)) {
			if (StackableItemInBackpack->GetClass() == StackableItem->GetClass() && StackableItemInBackpack != StackableItem)
			{
				if (StackableItemInBackpack->GetFreeStackSize() > 0)
				{
					return true;
				}
			}
		}
	}

	return false;
}

FIntPoint UInventoryComponent::Backpack_FindPlaceToPutItemTo(ABaseItem* Item)
{
	for (int i = 0; i <= BackpackSize.Y - Item->GetInventorySize().Y; ++i)
	{
		for (int j = 0; j <= BackpackSize.X - Item->GetInventorySize().X; ++j)
		{
			if (Backpack_CanPutItemTo(Item, j, i))
			{
				return FIntPoint(j, i);
			}
		}
	}

	return FIntPoint(-1, -1);
}

bool UInventoryComponent::Backpack_CanPutItemTo(ABaseItem* Item, int X, int Y)
{
	if (X < 0 || Y < 0
		|| X + Item->GetInventorySize().X > BackpackSize.X
		|| Y + Item->GetInventorySize().Y > BackpackSize.Y)
	{
		return false;
	}

	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item == Item)
		{
			continue;
		}

		FIntPoint L1 = ItemInBackpack.InventoryPosition;
		FIntPoint R1 = ItemInBackpack.InventoryPosition + ItemInBackpack.Item->GetInventorySize() - FIntPoint(1, 1);
		FIntPoint L2 = FIntPoint(X, Y);
		FIntPoint R2 = FIntPoint(X, Y) + Item->GetInventorySize() - FIntPoint(1, 1);

		if (RectanglesOverlap(L1, R1, L2, R2))
		{
			return false;
		}
	}

	return true;
}

void UInventoryComponent::Backpack_Server_PutItemTo_Implementation(ABaseItem* Item, int X, int Y)
{
	if (Backpack_CanPutItemTo(Item, X, Y))
	{
		RemoveFromPreviousSlot(Item, false);
		AddItemIfNeeded(Item);

		FBackpackItemWrapper BackpackItemWrapper = Backpack_GetItemWrapper(Item);
		if (BackpackItemWrapper == EMPTY_BACKPACK_ITEM_WRAPPER)
		{
			BackpackItemWrapper = FBackpackItemWrapper();
			BackpackItemWrapper.Item = Item;
			BackpackItemWrapper.InventoryPosition = FIntPoint(X, Y);
			ItemsInBackpack.Add(BackpackItemWrapper);
		} else {
			ItemsInBackpack.Remove(BackpackItemWrapper);
			BackpackItemWrapper.InventoryPosition = FIntPoint(X, Y);
			ItemsInBackpack.Add(BackpackItemWrapper);
		}

		Item->SetActorHiddenInGame(true);
		PlayerCharacter->Client_PlaySound(ItemMoveSound);
	}
}

bool UInventoryComponent::Backpack_Server_PutItemTo_Validate(ABaseItem* Item, int X, int Y)
{
	return true;
}

void UInventoryComponent::Backpack_Server_AddToStackAt_Implementation(ABaseStackableItem* StackableItem, int X, int Y)
{
	FBackpackItemWrapper ItemAtPosition = Backpack_GetItemAt(X, Y);

	if (ItemAtPosition != EMPTY_BACKPACK_ITEM_WRAPPER)
	{
		if (ABaseStackableItem* StackableItemInBackpack = dynamic_cast<ABaseStackableItem*>(ItemAtPosition.Item)) {
			if (StackableItemInBackpack->GetClass() == StackableItem->GetClass())
			{
				int StackIncreasedBy = StackableItemInBackpack->AddToStack(StackableItem->GetStackSize());
				StackableItem->RemoveFromStack(StackIncreasedBy);

				if (StackIncreasedBy > 0)
				{
					PlayerCharacter->Client_PlaySound(ItemMoveSound);
				}

				if (StackableItem->IsStackEmpty())
				{
					if (!StackableItem->GetOwner())
					{
						PlayerCharacter->Multicast_PlaySound(PickupSound);
					}
					Server_DestroyItem_Implementation(StackableItem);
					return;
				}
			}
		}
	}
}

bool UInventoryComponent::Backpack_Server_AddToStackAt_Validate(ABaseStackableItem* StackableItem, int X, int Y)
{
	return true;
}

void UInventoryComponent::Backpack_Server_AddToExistingStacks_Implementation(ABaseStackableItem* StackableItem)
{
	TArray<ABaseStackableItem*> ItemsOfType;

	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item->GetClass() == StackableItem->GetClass())
		{
			ItemsOfType.Add(Cast<ABaseStackableItem>(ItemInBackpack.Item));
		}
	}

	ItemsOfType.Sort([](ABaseStackableItem& LHS, ABaseStackableItem& RHS)
	{
		return LHS.GetStackSize() > RHS.GetStackSize();
	});

	for (ABaseStackableItem* ItemInBackpack : ItemsOfType)
	{
		int StackIncreasedBy = ItemInBackpack->AddToStack(StackableItem->GetStackSize());
		StackableItem->RemoveFromStack(StackIncreasedBy);

		if (StackIncreasedBy > 0)
		{
			PlayerCharacter->Client_PlaySound(ItemMoveSound);
		}
				
		if (StackableItem->IsStackEmpty())
		{
			Server_DestroyItem_Implementation(StackableItem);
			return;
		}
	}
}

bool UInventoryComponent::Backpack_Server_AddToExistingStacks_Validate(ABaseStackableItem* StackabeItem)
{
	return true;
}

void UInventoryComponent::Backpack_Server_RemoveFromExistingStacks_Implementation(TSubclassOf<ABaseStackableItem> StackableItemClass, int RemoveAmount)
{
	TArray<ABaseStackableItem*> ItemsOfType;

	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item->GetClass() == StackableItemClass.Get())
		{
			ItemsOfType.Add(Cast<ABaseStackableItem>(ItemInBackpack.Item));
		}
	}

	ItemsOfType.Sort([](ABaseStackableItem& LHS, ABaseStackableItem& RHS)
	{
		return LHS.GetStackSize() < RHS.GetStackSize();
	});

	for (ABaseStackableItem* ItemInBackpack : ItemsOfType)
	{
		RemoveAmount -= ItemInBackpack->RemoveFromStack(RemoveAmount);

		if (ItemInBackpack->IsStackEmpty())
		{
			Server_DestroyItem_Implementation(ItemInBackpack);
		}

		if (RemoveAmount <= 0)
		{
			return;
		}
	}
}

bool UInventoryComponent::Backpack_Server_RemoveFromExistingStacks_Validate(TSubclassOf<ABaseStackableItem> StackableItemClass, int RemoveAmount)
{
	return true;
}

int UInventoryComponent::Backpack_CountItemsOfType(TSubclassOf<ABaseItem> ItemType)
{
	int ItemsCount = 0;

	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item->GetClass() == ItemType.Get())
		{
			if (ABaseStackableItem* StackableItemInBackpack = dynamic_cast<ABaseStackableItem*>(ItemInBackpack.Item))
			{
				ItemsCount += StackableItemInBackpack->GetStackSize();
			}
			else
			{
				ItemsCount++;
			}
		}
	}

	return ItemsCount;
}

int UInventoryComponent::Backpack_CountStackableItemsOfType(TSubclassOf<ABaseStackableItem> ItemType)
{
	int ItemsCount = 0;

	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item && ItemInBackpack.Item->GetClass() == ItemType.Get())
		{
			ABaseStackableItem* StackableItemInBackpack = Cast<ABaseStackableItem>(ItemInBackpack.Item);
			ItemsCount += StackableItemInBackpack->GetStackSize();
		}
	}

	return ItemsCount;
}

void UInventoryComponent::Backpack_GetItemsOfType(TSubclassOf<ABaseItem> ItemType, TArray<ABaseItem*> &ReturnArray)
{
	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item->GetClass() == ItemType.Get())
		{
			ReturnArray.Add(Cast<ABaseItem>(ItemInBackpack.Item));
		}
	}

	if (ReturnArray.Num() > 0)
	{
		if (!dynamic_cast<ABaseStackableItem*>(ReturnArray[0])) {
			ReturnArray.Sort([](ABaseItem& LHS, ABaseItem& RHS)
			{
				ABaseStackableItem* StackableLHS = Cast<ABaseStackableItem>(&LHS);
				ABaseStackableItem* StackableRHS = Cast<ABaseStackableItem>(&RHS);
				return StackableLHS->GetStackSize() < StackableRHS->GetStackSize();
			});
		}
	}
}

void UInventoryComponent::Backpack_GetStackableItemsOfType(TSubclassOf<ABaseStackableItem> ItemType, TArray<ABaseStackableItem*> &ReturnArray)
{
	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		if (ItemInBackpack.Item->GetClass() == ItemType.Get())
		{
			ReturnArray.Add(Cast<ABaseStackableItem>(ItemInBackpack.Item));
		}
	}

	ReturnArray.Sort([](ABaseStackableItem& LHS, ABaseStackableItem& RHS)
	{
		return LHS.GetStackSize() < RHS.GetStackSize();
	});
}

void UInventoryComponent::Armor_Server_PutItem_Implementation(AArmorItem* TheArmorItem)
{
	RemoveFromPreviousSlot(TheArmorItem, false);
	AddItemIfNeeded(TheArmorItem);

	if (ArmorItem)
	{
		FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(ArmorItem);
		if (PlaceToPutItemTo.X != -1 && PlaceToPutItemTo.Y != -1)
		{
			Backpack_Server_PutItemTo(ArmorItem, PlaceToPutItemTo.X, PlaceToPutItemTo.Y);
		} else {
			Server_DropItem(ArmorItem);
		}
	}

	ArmorItem = TheArmorItem;
	TheArmorItem->SetActorHiddenInGame(true);
	PlayerCharacter->Client_PlaySound(ItemMoveSound);

	PlayerCharacter->SetArmorItem(ArmorItem);
}

bool UInventoryComponent::Armor_Server_PutItem_Validate(AArmorItem* TheArmorItem)
{
	return true;
}

void UInventoryComponent::SetWeaponItem(ABaseWeaponItem* TheWeaponItem)
{
	if (WeaponItem && WeaponItem != TheWeaponItem)
	{
		if (AFirearmWeaponItem* FirearmWeaponItem = dynamic_cast<AFirearmWeaponItem*>(WeaponItem)) {
			FirearmWeaponItem->CancelReload();
		}
		WeaponItem->SetActorHiddenInGame(true);
	}

	if (TheWeaponItem)
	{
		TheWeaponItem->SetActorHiddenInGame(false);
	}

	WeaponItem = TheWeaponItem;
	PlayerCharacter->SetWeaponItem(WeaponItem);
}

void UInventoryComponent::Server_DropAllItems_Implementation()
{
	TArray<ABaseItem*> AllItems_Copy = TArray<ABaseItem*>();
	for (ABaseItem* Item : AllItems)
	{
		AllItems_Copy.Add(Item);
	}
	for (ABaseItem* Item : AllItems_Copy)
	{
		Server_DropItem(Item);
	}
}

bool UInventoryComponent::Server_DropAllItems_Validate()
{
	return true;
}

void UInventoryComponent::Server_RemoveFromInventory_Implementation(ABaseItem* Item)
{
	RemoveFromPreviousSlot(Item, false);
	AllItems.Remove(Item);
	Client_OnItemDropped(Item);
}

bool UInventoryComponent::Server_RemoveFromInventory_Validate(ABaseItem* Item)
{
	return true;
}

void UInventoryComponent::Server_DestroyItem_Implementation(ABaseItem* Item)
{
	RemoveFromPreviousSlot(Item, false);
	AllItems.Remove(Item);
	Client_OnItemDropped(Item);
	Item->Destroy();
}

bool UInventoryComponent::Server_DestroyItem_Validate(ABaseItem* Item)
{
	return false;
}

void UInventoryComponent::Server_OnItemUsed_Implementation(ABaseItem* Item)
{
	if (AArmorItem* TheArmorItem = dynamic_cast<AArmorItem*>(Item))
	{
		if (TheArmorItem == ArmorItem)
		{
			FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(TheArmorItem);
			Backpack_Server_PutItemTo(TheArmorItem, PlaceToPutItemTo.X, PlaceToPutItemTo.Y);
		}
		else
		{
			Armor_Server_PutItem(TheArmorItem);
		}

		return;
	}
	else if (ABaseWeaponItem* TheWeaponItem = dynamic_cast<ABaseWeaponItem*>(Item))
	{
		if (TheWeaponItem == WeaponSlot1Item || TheWeaponItem == WeaponSlot2Item)
		{
			FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(TheWeaponItem);
			Backpack_Server_PutItemTo(TheWeaponItem, PlaceToPutItemTo.X, PlaceToPutItemTo.Y);
		}
		else
		{
			if (WeaponSlot1Item)
			{
				if (WeaponSlot2Item)
				{
					Weapon1_Server_PutItem(TheWeaponItem);
				}
				else
				{
					Weapon2_Server_PutItem(TheWeaponItem);
				}
			}
			else
			{
				Weapon1_Server_PutItem(TheWeaponItem);
			}
		}

		return;
	}
	else if (ABaseStackableItem* TheStackableItem = dynamic_cast<ABaseStackableItem*>(Item))
	{
		TheStackableItem->RemoveFromStack(1);
		if (TheStackableItem->IsStackEmpty())
		{
			Server_DestroyItem_Implementation(TheStackableItem);
		}
	}
	else
	{
		Server_DestroyItem_Implementation(Item);
	}
}

bool UInventoryComponent::Server_OnItemUsed_Validate(ABaseItem* Item)
{
	return false;
}

void UInventoryComponent::Server_UseWeapon_Implementation(ABaseWeaponItem* TheWeaponItem)
{
	if (WeaponItem != TheWeaponItem)
	{
		if (!TheWeaponItem)
		{
			SetWeaponItem(TheWeaponItem);
		}
		else
		{
			if (TheWeaponItem == WeaponSlot1Item || TheWeaponItem == WeaponSlot2Item || TheWeaponItem == WeaponSlot3Item)
			{
				SetWeaponItem(TheWeaponItem);
			}
			else
			{
				if (WeaponSlot1Item)
				{
					SetWeaponItem(WeaponSlot1Item);
				}
				else if (WeaponSlot2Item)
				{
					SetWeaponItem(WeaponSlot2Item);
				}
				else if (WeaponSlot3Item)
				{
					SetWeaponItem(WeaponSlot3Item);
				}
			}
		}
	}
}

bool UInventoryComponent::Server_UseWeapon_Validate(ABaseWeaponItem* TheWeaponItem)
{
	return true;
}

ABaseWeaponItem* UInventoryComponent::GetWeapon1()
{
	return WeaponSlot1Item;
}

ABaseWeaponItem* UInventoryComponent::GetWeapon2()
{
	return WeaponSlot2Item;
}

ABaseWeaponItem* UInventoryComponent::GetWeapon3()
{
	return WeaponSlot3Item;
}

void UInventoryComponent::Weapon1_Server_PutItem_Implementation(ABaseWeaponItem* TheWeaponItem)
{
	ABaseWeaponItem* OldWeaponItem = WeaponItem;

	if (TheWeaponItem)
	{
		RemoveFromPreviousSlot(TheWeaponItem, true);
		AddItemIfNeeded(TheWeaponItem);

		if (WeaponSlot1Item)
		{
			if (!WeaponSlot2Item)
			{
				Weapon2_Server_PutItem(WeaponSlot1Item);
			}
			else
			{
				FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(WeaponSlot1Item);
				if (PlaceToPutItemTo.X != -1 && PlaceToPutItemTo.Y != -1)
				{
					Backpack_Server_PutItemTo(WeaponSlot1Item, PlaceToPutItemTo.X, PlaceToPutItemTo.Y);
				}
				else
				{
					Server_DropItem(WeaponSlot1Item);
				}
			}
		}

		WeaponSlot1Item = TheWeaponItem;
		TheWeaponItem->SetActorHiddenInGame(true);
		PlayerCharacter->Client_PlaySound(ItemMoveSound);

		if (OldWeaponItem == TheWeaponItem)
		{
			SetWeaponItem(OldWeaponItem);
		}
		else if (!WeaponItem)
		{
			SetWeaponItem(WeaponSlot1Item);
		}
	}
}

bool UInventoryComponent::Weapon1_Server_PutItem_Validate(ABaseWeaponItem* TheWeaponItem)
{
	return true;
}

void UInventoryComponent::Weapon2_Server_PutItem_Implementation(ABaseWeaponItem* TheWeaponItem)
{
	ABaseWeaponItem* OldWeaponItem = WeaponItem;

	if (TheWeaponItem)
	{
		RemoveFromPreviousSlot(TheWeaponItem, true);
		AddItemIfNeeded(TheWeaponItem);

		if (WeaponSlot2Item)
		{
			if (!WeaponSlot1Item)
			{
				Weapon1_Server_PutItem(WeaponSlot2Item);
			}
			else
			{
				FIntPoint PlaceToPutItemTo = Backpack_FindPlaceToPutItemTo(WeaponSlot2Item);
				if (PlaceToPutItemTo.X != -1 && PlaceToPutItemTo.Y != -1)
				{
					Backpack_Server_PutItemTo(WeaponSlot2Item, PlaceToPutItemTo.X, PlaceToPutItemTo.Y);
				}
				else
				{
					Server_DropItem(WeaponSlot2Item);
				}
			}
		}

		WeaponSlot2Item = TheWeaponItem;
		TheWeaponItem->SetActorHiddenInGame(true);
		PlayerCharacter->Client_PlaySound(ItemMoveSound);

		if (OldWeaponItem == TheWeaponItem)
		{
			SetWeaponItem(OldWeaponItem);
		}
		else if (!WeaponItem)
		{
			SetWeaponItem(WeaponSlot2Item);
		}
	}
}

bool UInventoryComponent::Weapon2_Server_PutItem_Validate(ABaseWeaponItem* TheWeaponItem)
{
	return true;
}




// ------------------------------------------------------------------------- Etc --------------------------------------------------------------------------

void UInventoryComponent::Client_OnItemPickedUp_Implementation(ABaseItem* Item)
{
	if (Item)
	{
		Item->CreateItemWidget(InventoryUI);
	}
}

void UInventoryComponent::Client_OnItemDropped_Implementation(ABaseItem* Item)
{
	if (Item)
	{
		Item->RemoveItemWidget();
	}
}

void UInventoryComponent::OnRep_ItemsInBackpack()
{
	if (InventoryUI)
	{
		for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
		{
			Client_OnItemPickedUp_Implementation(ItemInBackpack.Item);
			OnItemUpdate(ItemInBackpack.Item);
		}
	}
}

void UInventoryComponent::OnRep_ArmorItem()
{
	if (InventoryUI && WeaponSlot1Item)
	{
		Client_OnItemPickedUp_Implementation(ArmorItem);
		OnItemUpdate(ArmorItem);
	}
}

void UInventoryComponent::OnRep_WeaponSlot1Item()
{
	if (InventoryUI && WeaponSlot1Item)
	{
		Client_OnItemPickedUp_Implementation(WeaponSlot1Item);
		OnItemUpdate(WeaponSlot1Item);
	}
}

void UInventoryComponent::OnRep_WeaponSlot2Item()
{
	if (InventoryUI && WeaponSlot2Item)
	{
		Client_OnItemPickedUp_Implementation(WeaponSlot2Item);
		OnItemUpdate(WeaponSlot2Item);
	}
}

void UInventoryComponent::SetPickupSphere(USphereComponent* ThePickupSphere)
{
	PickupSphere = ThePickupSphere;

	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &UInventoryComponent::OnItemBeginOverlap);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &UInventoryComponent::OnItemEndOverlap);
}

void UInventoryComponent::OnPossess()
{
	for (ABaseItem* Item : AllItems)
	{
		Item->SetOwner(GetOwner());
		Item->SetInventoryComponent(this);
		Item->SetPlayerCharacter(PlayerCharacter);
		Item->SetPickable(false);
	}
}

void UInventoryComponent::SetInventoryUI(UInventoryUI* TheInventoryUI)
{
	InventoryUI = TheInventoryUI;

	for (FBackpackItemWrapper ItemInBackpack : ItemsInBackpack)
	{
		Client_OnItemPickedUp_Implementation(ItemInBackpack.Item);
		OnItemUpdate(ItemInBackpack.Item);
	}

	if (ArmorItem)
	{
		Client_OnItemPickedUp_Implementation(ArmorItem);
		OnItemUpdate(ArmorItem);
	}

	if (WeaponSlot1Item)
	{
		Client_OnItemPickedUp_Implementation(WeaponSlot1Item);
		OnItemUpdate(WeaponSlot1Item);
	}

	if (WeaponSlot2Item)
	{
		Client_OnItemPickedUp_Implementation(WeaponSlot2Item);
		OnItemUpdate(WeaponSlot2Item);
	}
}

void UInventoryComponent::OnItemUpdate(ABaseItem* Item)
{
	if (Item)
	{
		FBackpackItemWrapper BackpackItemWrapper = Backpack_GetItemWrapper(Item);
		if (BackpackItemWrapper == EMPTY_BACKPACK_ITEM_WRAPPER)
		{} else {
			InventoryUI->PutItemToBackpack(Item->GetItemWidget(), BackpackItemWrapper.InventoryPosition);

			return;
		}

		if (Item == ArmorItem)
		{
			InventoryUI->PutItemToArmorSlot(Item->GetItemWidget());

			return;
		}

		if (Item == WeaponSlot1Item)
		{
			InventoryUI->PutItemToWeapon1Slot(Item->GetItemWidget());

			return;
		}

		if (Item == WeaponSlot2Item)
		{
			InventoryUI->PutItemToWeapon2Slot(Item->GetItemWidget());

			return;
		}
	}
}

void UInventoryComponent::DestroyAllItems()
{
	for (ABaseItem* Item : AllItems)
	{
		Item->Destroy();
	}
}
