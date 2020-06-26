// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableItemsWidget.h"

#include "StalkerMP/Items/HUD/ItemWidget.h"

#include "Components/PanelWidget.h"


void UPickableItemsWidget::OnPickableItemAppeared(UItemWidget* ItemWidget)
{
	GetPickableItemsBox()->AddChild((UWidget*)ItemWidget);
	ItemWidget->FixIconSize();
}

void UPickableItemsWidget::OnPickableItemGone(UItemWidget* ItemWidget)
{
	GetPickableItemsBox()->RemoveChild((UWidget*)ItemWidget);
}
