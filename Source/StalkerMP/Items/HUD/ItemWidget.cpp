// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemWidget.h"

#include "StalkerMP/Items/HUD/InventoryUI.h"
#include "StalkerMP/Items/HUD/ItemDragWidget.h"
#include "StalkerMP/Items/BaseItem.h"
#include "StalkerMP/Items/BaseStackableItem.h"
#include "StalkerMP/UI/TooltipWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/DragDropOperation.h"




// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	} else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (Item->GetOwner() == GetOwningPlayerPawn())
		{
			if (Item->IsCanBeUsed())
			{
				Item->Server_Use();
			}
		}
		else
		{
			Item->Pickup();
		}
		return UWidgetBlueprintLibrary::Handled().NativeReply;
	}

	return UWidgetBlueprintLibrary::Unhandled().NativeReply;
}

FReply UItemWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (Item->GetOwner() == GetOwningPlayerPawn())
		{
			if (Item->IsCanBeUsed())
			{
				Item->Server_Use();
			}
		}
		else
		{
			Item->Pickup();
		}
		return UWidgetBlueprintLibrary::Handled().NativeReply;
	}

	return UWidgetBlueprintLibrary::Unhandled().NativeReply;
}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UItemDragWidget* ItemDragWidget = CreateWidget<UItemDragWidget>(GetGameInstance(), ItemDragWidgetClass.Get());
	ItemDragWidget->SetIconImage(Item->GetIconImageTexture(), IconImageSize);
	ItemDragWidget->DragOffset = DragOffset;

	UDragDropOperation* DragDropOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());

	DragDropOperation->Payload = this;
	DragDropOperation->DefaultDragVisual = ItemDragWidget;
	DragDropOperation->Pivot = EDragPivot::MouseDown;
	DragDropOperation->Offset = FVector2D(0, 0);

	OutOperation = DragDropOperation;
}




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

void UItemWidget::SetItem(ABaseItem* TheItem)
{
	Item = TheItem;

	IconImageSize = FVector2D(Item->GetInventorySize().X * UInventoryUI::CELL_SIZE_PIXELS, Item->GetInventorySize().Y * UInventoryUI::CELL_SIZE_PIXELS);
	SetIconImage(Item->GetIconImageTexture(), IconImageSize);
	FixIconSize();

	if (ABaseStackableItem* TheStackableItem = dynamic_cast<ABaseStackableItem*>(Item)) {
		StackableItem = TheStackableItem;
	}

	UTooltipWidget* TooltipWidget = CreateWidget<UTooltipWidget>(GetOwningPlayer(), Item->TooltipWidgetClass.Get());
	TooltipWidget->SetTexts(Item->GetItemName(), Item->GetItemDetails());
	SetToolTip(TooltipWidget);
}

void UItemWidget::FixIconSize()
{
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetSize(IconImageSize);
	}
	else
	{

	}
}

int UItemWidget::GetStackSize()
{
	if (StackableItem) {
		return StackableItem->GetStackSize();
	}

	return -1;
}