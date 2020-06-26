// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadBodyBackpack.h"

#include "StalkerMP/Items/HUD/InventoryUI.h"
#include "StalkerMP/Items/BaseItem.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine.h"



ADeadBodyBackpack::ADeadBodyBackpack()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("PickableItem"));
	//StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->CanCharacterStepUpOn = ECB_No;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->bCastStaticShadow = false;
	RootComponent = StaticMeshComponent;

	ActionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ActionCollision"));
	ActionCollision->SetCollisionProfileName(TEXT("ActionReceiver"));
	ActionCollision->CanCharacterStepUpOn = ECB_No;
	ActionCollision->SetGenerateOverlapEvents(true);
	ActionCollision->SetCanEverAffectNavigation(false);
	ActionCollision->SetupAttachment(RootComponent);
}

void ADeadBodyBackpack::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeadBodyBackpack, AllItems);
	DOREPLIFETIME(ADeadBodyBackpack, BackpackSize);
	DOREPLIFETIME(ADeadBodyBackpack, ItemsInBackpack);
	DOREPLIFETIME(ADeadBodyBackpack, ArmorItem);
	DOREPLIFETIME(ADeadBodyBackpack, WeaponSlot1Item);
	DOREPLIFETIME(ADeadBodyBackpack, WeaponSlot2Item);
}

void ADeadBodyBackpack::SetInventoryUI(UInventoryUI* TheInventoryUI)
{
	InventoryUI = TheInventoryUI;
}

void ADeadBodyBackpack::GetAllItems(TArray<ABaseItem*> &ArrayToFill)
{
	ArrayToFill = AllItems;
}

void ADeadBodyBackpack::Multicast_RemoveItem_Implementation(ABaseItem* Item)
{
	AllItems.Remove(Item);

	for (int i = 0; i < ItemsInBackpack.Num(); i++)
	{
		if (ItemsInBackpack[i].Item == Item)
		{
			ItemsInBackpack.RemoveAt(i);
			break;
		}
	}
	if (Item == ArmorItem)
	{
		ArmorItem = nullptr;
	}
	if (Item == WeaponSlot1Item)
	{
		WeaponSlot1Item = nullptr;
	}
	if (Item == WeaponSlot2Item)
	{
		WeaponSlot2Item = nullptr;
	}

	if (InventoryUI && Item->GetItemWidget())
	{
		InventoryUI->OnDeadBodyItemGone(Item->GetItemWidget());
	}
}

void ADeadBodyBackpack::AddItem(ABaseItem* Item) {
	AllItems.Add(Item);
}

void ADeadBodyBackpack::SetBackpackSize(FIntPoint TheBackpackSize)
{
	BackpackSize = TheBackpackSize;
}

void ADeadBodyBackpack::AddItemToBackpack(ABaseItem* Item, FIntPoint ItemPosition)
{
	Item->SetDeadBodyBackpack(this);

	FDeadBodyBackpackItemWrapper DeadBodyBackpackItemWrapper;
	DeadBodyBackpackItemWrapper.Item = Item;
	DeadBodyBackpackItemWrapper.InventoryPosition = ItemPosition;
	ItemsInBackpack.Add(DeadBodyBackpackItemWrapper);
}

FIntPoint ADeadBodyBackpack::GetBackpackSize()
{
	return BackpackSize;
}

void ADeadBodyBackpack::GetItemsInBackpack(TArray<FDeadBodyBackpackItemWrapper> &ArrayToFill)
{
	ArrayToFill = ItemsInBackpack;
}

void ADeadBodyBackpack::SetArmorItem(ABaseItem* TheArmorItem)
{
	ArmorItem = TheArmorItem;
	ArmorItem->SetDeadBodyBackpack(this);
}

ABaseItem* ADeadBodyBackpack::GetArmorItem()
{
	return ArmorItem;
}

void ADeadBodyBackpack::SetWeaponSlot1Item(ABaseItem* TheWeaponSlot1Item)
{
	WeaponSlot1Item = TheWeaponSlot1Item;
	WeaponSlot1Item->SetDeadBodyBackpack(this);
}

ABaseItem* ADeadBodyBackpack::GetWeaponSlot1Item()
{
	return WeaponSlot1Item;
}

void ADeadBodyBackpack::SetWeaponSlot2Item(ABaseItem* TheWeaponSlot2Item)
{
	WeaponSlot2Item = TheWeaponSlot2Item;
	WeaponSlot2Item->SetDeadBodyBackpack(this);
}

ABaseItem* ADeadBodyBackpack::GetWeaponSlot2Item()
{
	return WeaponSlot2Item;
}

void ADeadBodyBackpack::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		for (ABaseItem* Item : AllItems)
		{
			Item->Destroy();
		}
	}

	Super::EndPlay(EndPlayReason);
}
