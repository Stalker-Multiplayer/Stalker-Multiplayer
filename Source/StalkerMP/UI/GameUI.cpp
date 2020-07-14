// Fill out your copyright notice in the Description page of Project Settings.

#include "GameUI.h"

#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/UI/Map/MinimapWidget.h"
#include "StalkerMP/UI/Map/MapWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Public/TimerManager.h"

void UGameUI::SetPlayerController(ABasePlayerController* ThePlayerController)
{
	if (ThePlayerController->MinimapWidgetClass.Get())
	{
		MinimapWidget = (UMinimapWidget*)CreateWidget(this, ThePlayerController->MinimapWidgetClass);
		MinimapWidget->SetPlayerController(ThePlayerController);
		UCanvasPanelSlot* MinimapSlot = GetRootCanvas()->AddChildToCanvas(MinimapWidget);
		MinimapSlot->SetPosition(FVector2D(32, 32));
	}

	if (ThePlayerController->MapWidgetClass.Get())
	{
		MapWidget = (UMapWidget*)CreateWidget(this, ThePlayerController->MapWidgetClass);
		MapWidget->SetPlayerController(ThePlayerController);
		UCanvasPanelSlot* MapSlot = GetRootCanvas()->AddChildToCanvas(MapWidget);
		MapSlot->SetAlignment(FVector2D(0.5, 0.5));
		MapSlot->SetAnchors(FAnchors(0.5, 0.5, 0.5, 0.5));
	}

	HideMap();
}

void UGameUI::MapZoomIn()
{
	if (MapWidget)
	{
		MapWidget->ZoomIn();
	}
}

void UGameUI::MapZoomOut()
{
	if (MapWidget)
	{
		MapWidget->ZoomOut();
	}
}

bool UGameUI::IsMapVisible()
{
	if (MapWidget)
	{
		return MapWidget->GetVisibility() == ESlateVisibility::Visible;
	}
	else
	{
		return false;
	}
}

void UGameUI::ShowMap()
{
	if (MinimapWidget)
	{
		MinimapWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (MapWidget)
	{
		MapWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameUI::HideMap()
{
	if (MinimapWidget)
	{
		MinimapWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (MapWidget)
	{
		MapWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGameUI::OnPlayerDied(FString PlayerName, FString KillerName, bool KilledByMe, bool MeDied)
{
	GetWorld()->GetTimerManager().ClearTimer(KilledPlayerVisibleTimerHandler);
	GetWorld()->GetTimerManager().SetTimer(KilledPlayerVisibleTimerHandler, this, &UGameUI::HideDiedPlayer, KilledPlayerVisibilityTime, false);
	OnPlayerDiedBP(PlayerName, KillerName, KilledByMe, MeDied);
}
