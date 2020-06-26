// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryUI.h"

#include "StalkerMP/Items/BaseItem.h"
#include "StalkerMP/Items/HUD/ItemWidget.h"
#include "StalkerMP/Items/HUD/PickableItemsWidget.h"
#include "StalkerMP/Items/HUD/InventorySlotWidget.h"
#include "StalkerMP/Items/HUD/InventoryBackpackWidget.h"
#include "StalkerMP/Items/HUD/DeadBodyInventoryUI.h"
#include "StalkerMP/Items/InventoryComponent.h"
#include "StalkerMP/Items/DeadBodyBackpack.h"

#include "Blueprint/DragDropOperation.h"




// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

void UInventoryUI::NativeConstruct()
{
	GetInventoryBackpackWidget()->SetInventoryComponent(InventoryComponent);
	GetInventoryBackpackWidget()->SetBackpackSize(InventoryComponent->GetBackpackSize());

	GetArmorInventorySlotWidget()->SetInventoryComponent(InventoryComponent);
	GetWeapon1InventorySlotWidget()->SetInventoryComponent(InventoryComponent);
	GetWeapon2InventorySlotWidget()->SetInventoryComponent(InventoryComponent);

	GetDeadBodyInventoryUI()->SetInventoryComponent(InventoryComponent);

	SetDeadBodyBackpack(nullptr);
}

bool UInventoryUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UItemWidget* ItemWidget = dynamic_cast<UItemWidget*>(InOperation->Payload)) {
		if (!ItemWidget->GetItem()->GetOwner())
		{
			return true;
		}

		InventoryComponent->Server_DropItem(ItemWidget->GetItem());

		return true;
	}

	return false;
}



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

void UInventoryUI::OnPickableItemAppeared(UItemWidget* ItemWidget)
{
	GetPickableItemsWidget()->OnPickableItemAppeared(ItemWidget);
}

void UInventoryUI::OnPickableItemGone(UItemWidget* ItemWidget)
{
	GetPickableItemsWidget()->OnPickableItemGone(ItemWidget);
}

void UInventoryUI::OnDeadBodyItemGone(UItemWidget* ItemWidget)
{
	GetDeadBodyInventoryUI()->OnItemGone(ItemWidget);
}

void UInventoryUI::PutItemToBackpack(UItemWidget* ItemWidget, FIntPoint Position)
{
	GetInventoryBackpackWidget()->AddItemWidget(ItemWidget, Position);
}

void UInventoryUI::PutItemToArmorSlot(UItemWidget* ItemWidget)
{
	GetArmorInventorySlotWidget()->AddItemWidget(ItemWidget);
}

void UInventoryUI::PutItemToWeapon1Slot(UItemWidget* ItemWidget)
{
	GetWeapon1InventorySlotWidget()->AddItemWidget(ItemWidget);
}

void UInventoryUI::PutItemToWeapon2Slot(UItemWidget* ItemWidget)
{
	GetWeapon2InventorySlotWidget()->AddItemWidget(ItemWidget);
}

void UInventoryUI::SetDeadBodyBackpack(ADeadBodyBackpack* TheDeadBodyBackpack)
{
	if (DeadBodyBackpack && DeadBodyBackpack != TheDeadBodyBackpack)
	{
		TArray<ABaseItem*> AllItems;
		DeadBodyBackpack->GetAllItems(AllItems);
		for (int i = 0; i < AllItems.Num(); i++)
		{
			AllItems[i]->RemoveItemWidget();
			AllItems[i]->SetInventoryComponentForPickup(nullptr);
		}
	}

	DeadBodyBackpack = TheDeadBodyBackpack;

	if (DeadBodyBackpack)
	{
		TArray<ABaseItem*> AllItems;
		DeadBodyBackpack->GetAllItems(AllItems);
		for (int i = 0; i < AllItems.Num(); i++)
		{
			AllItems[i]->CreateItemWidget(this);
			AllItems[i]->SetInventoryComponentForPickup(InventoryComponent);
		}

		GetPickableItemsWidget()->SetVisibility(ESlateVisibility::Collapsed);
		GetDeadBodyInventoryUI()->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GetPickableItemsWidget()->SetVisibility(ESlateVisibility::Visible);
		GetDeadBodyInventoryUI()->SetVisibility(ESlateVisibility::Collapsed);
	}

	GetDeadBodyInventoryUI()->SetDeadBodyBackpack(DeadBodyBackpack);
}

bool UInventoryUI::IsDeadBodyInventoryUIVisible()
{
	return GetDeadBodyInventoryUI()->GetVisibility() == ESlateVisibility::Visible;
}
