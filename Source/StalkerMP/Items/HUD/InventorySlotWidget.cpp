// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlotWidget.h"

#include "StalkerMP/Items/HUD/ItemWidget.h"
#include "StalkerMP/Items/HUD/ItemDragWidget.h"
#include "StalkerMP/Items/ArmorItem.h"
#include "StalkerMP/Items/BaseWeaponItem.h"
#include "StalkerMP/Items/InventoryComponent.h"

#include "Components/CanvasPanel.h"
#include "Blueprint/DragDropOperation.h"



// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InventoryComponent)
	{
		if (UItemWidget* ItemWidget = dynamic_cast<UItemWidget*>(InOperation->Payload)) {
			if (UItemDragWidget* DragWidget = dynamic_cast<UItemDragWidget*>(InOperation->DefaultDragVisual)) {
				switch (SlotType)
				{
				case ESlotType::ST_Armor: {
					if (AArmorItem* ArmorItem = dynamic_cast<AArmorItem*>(ItemWidget->GetItem())) {
						InventoryComponent->Armor_Server_PutItem(ArmorItem);
					}
				}
										  break;
				case ESlotType::ST_Weapon1: {
					if (ABaseWeaponItem* WeaponItem = dynamic_cast<ABaseWeaponItem*>(ItemWidget->GetItem())) {
						InventoryComponent->Weapon1_Server_PutItem(WeaponItem);
					}
				}
											break;
				case ESlotType::ST_Weapon2: {
					if (ABaseWeaponItem* WeaponItem = dynamic_cast<ABaseWeaponItem*>(ItemWidget->GetItem())) {
						InventoryComponent->Weapon2_Server_PutItem(WeaponItem);
					}
				}
											break;
				}

				return true;
			}
		}
	}

	return false;
}



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

void UInventorySlotWidget::AddItemWidget(UItemWidget* ItemWidget)
{
	ItemWidget->RemoveFromParent();
	GetCanvasPanel()->AddChildToCanvas(ItemWidget);
	ItemWidget->FixIconSize();
}