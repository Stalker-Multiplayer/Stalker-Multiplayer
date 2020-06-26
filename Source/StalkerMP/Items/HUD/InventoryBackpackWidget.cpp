// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryBackpackWidget.h"

#include "StalkerMP/Items/BaseStackableItem.h"
#include "StalkerMP/Items/HUD/InventoryUI.h"
#include "StalkerMP/Items/HUD/ItemWidget.h"
#include "StalkerMP/Items/HUD/ItemDragWidget.h"
#include "StalkerMP/Items/InventoryComponent.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/CanvasPanel.h"




// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

bool UInventoryBackpackWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InventoryComponent)
	{
		if (UItemWidget* ItemWidget = dynamic_cast<UItemWidget*>(InOperation->Payload)) {
			if (UItemDragWidget* DragWidget = dynamic_cast<UItemDragWidget*>(InOperation->DefaultDragVisual)) {

				FVector2D Position = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - DragWidget->DragOffset;

				int X = round(Position.X / UInventoryUI::CELL_SIZE_PIXELS);
				int Y = round(Position.Y / UInventoryUI::CELL_SIZE_PIXELS);

				if (ABaseStackableItem* TheStackableItem = dynamic_cast<ABaseStackableItem*>(ItemWidget->GetItem()))
				{
					if (InventoryComponent->Backpack_CanAddToStackAt(TheStackableItem, X, Y))
					{
						InventoryComponent->Backpack_Server_AddToStackAt(TheStackableItem, X, Y);
						return true;
					}
				}

				if (InventoryComponent->Backpack_CanPutItemTo(ItemWidget->GetItem(), X, Y))
				{
					InventoryComponent->Backpack_Server_PutItemTo(ItemWidget->GetItem(), X, Y);
				}

				return true;
			}
		}
	}

	return false;
}




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

void UInventoryBackpackWidget::SetBackpackSize(FIntPoint InventorySize)
{
	CellWidgetsArray = new UUserWidget**[InventorySize.X];
	for (int i = 0; i < InventorySize.X; ++i)
	{
		CellWidgetsArray[i] = new UUserWidget*[InventorySize.Y];

		for (int j = 0; j < InventorySize.Y; ++j)
		{
			UUserWidget* CellWidget = CreateWidget<UUserWidget>(GetGameInstance(), CellWidgetClass.Get());
			UCanvasPanelSlot* CanvasPanelSlot = GetCanvasPanel()->AddChildToCanvas(CellWidget);
			CanvasPanelSlot->SetSize(FVector2D(UInventoryUI::CELL_SIZE_PIXELS, UInventoryUI::CELL_SIZE_PIXELS));
			CanvasPanelSlot->SetPosition(FVector2D(UInventoryUI::CELL_SIZE_PIXELS * i, UInventoryUI::CELL_SIZE_PIXELS * j));

			CellWidgetsArray[i][j] = CellWidget;
		}
	}
}

void UInventoryBackpackWidget::AddItemWidget(UItemWidget* ItemWidget, FIntPoint Position)
{
	ItemWidget->RemoveFromParent();
	GetCanvasPanel()->AddChildToCanvas(ItemWidget);
	ItemWidget->FixIconSize();
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemWidget);
	CanvasPanelSlot->SetPosition(FVector2D(UInventoryUI::CELL_SIZE_PIXELS * Position.X, UInventoryUI::CELL_SIZE_PIXELS * Position.Y));
}