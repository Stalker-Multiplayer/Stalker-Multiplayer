// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadBodyInventoryUI.h"

#include "StalkerMP/Items/HUD/ItemWidget.h"
#include "StalkerMP/Items/HUD/InventorySlotWidget.h"
#include "StalkerMP/Items/HUD/InventoryBackpackWidget.h"
#include "StalkerMP/Items/DeadBodyBackpack.h"
#include "StalkerMP/Items/BaseItem.h"


void UDeadBodyInventoryUI::SetDeadBodyBackpack(ADeadBodyBackpack* DeadBodyBackpack)
{
	if (DeadBodyBackpack)
	{
		GetInventoryBackpackWidget()->SetBackpackSize(DeadBodyBackpack->GetBackpackSize());
		TArray<FDeadBodyBackpackItemWrapper> ItemsInBackpack;
		DeadBodyBackpack->GetItemsInBackpack(ItemsInBackpack);
		for (int i = 0; i < ItemsInBackpack.Num(); i++)
		{
			GetInventoryBackpackWidget()->AddItemWidget(ItemsInBackpack[i].Item->GetItemWidget(), ItemsInBackpack[i].InventoryPosition);
		}

		ABaseItem* ArmorItem = DeadBodyBackpack->GetArmorItem();
		if (ArmorItem)
		{
			GetArmorInventorySlotWidget()->AddItemWidget(ArmorItem->GetItemWidget());
		}

		ABaseItem* WeaponSlot1Item = DeadBodyBackpack->GetWeaponSlot1Item();
		if (WeaponSlot1Item)
		{
			GetWeapon1InventorySlotWidget()->AddItemWidget(WeaponSlot1Item->GetItemWidget());
		}

		ABaseItem* WeaponSlot2Item = DeadBodyBackpack->GetWeaponSlot2Item();
		if (WeaponSlot2Item)
		{
			GetWeapon2InventorySlotWidget()->AddItemWidget(WeaponSlot2Item->GetItemWidget());
		}
	}
}

void UDeadBodyInventoryUI::OnItemGone(UItemWidget* ItemWidget)
{
	ItemWidget->RemoveFromParent();
}
