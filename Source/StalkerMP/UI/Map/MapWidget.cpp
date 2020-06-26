// Fill out your copyright notice in the Description page of Project Settings.

#include "MapWidget.h"

#include "StalkerMP/DataClasses/BaseLevelDataActor.h"
#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/UI/Map/MapPOIActor.h"
#include "StalkerMP/UI/Map/MapPOIWidget.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/UserInterfaceSettings.h"
#include "Engine/RendererSettings.h"
#include "Engine.h"

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseLevelDataActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ABaseLevelDataActor* LevelDataActor = (ABaseLevelDataActor*)FoundActors[0];

		SetMaterial(LevelDataActor->MapMaterial, LevelDataActor->MapTextureSize);

		CmPerPixel = LevelDataActor->MapCmPerPixel;
		TextureSize = LevelDataActor->MapTextureSize;
		TextureOffset = LevelDataActor->MapTextureOffset;
		Zoom = LevelDataActor->MapDefaultZoom;
		SetZoom(Zoom);

		UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
		if (CanvasPanelSlot)
		{
			CanvasPanelSlot->SetSize(TextureSize);
		}

		SetIconsScale(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
			ESettingsType::General,
			ABasePlayerController::MAP_ICONS_SCALE_SETTING_KEY,
			ABasePlayerController::DEFAULT_MINIMAP_ICONS_SIZE));
	}

	GetWorld()->GetTimerManager().SetTimer(CreatePOIWidgetsTimerHandle, this, &UMapWidget::CreatePOIWidgets, 1, false, 0.0);

	OnFloatSettingValueUpdatedDelegate.BindUFunction(this, "OnFloatSettingUpdated");
	UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.AddUnique(OnFloatSettingValueUpdatedDelegate);
}

void UMapWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.Remove(OnFloatSettingValueUpdatedDelegate);
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (PlayerController->GetPawn())
	{
		FVector PlayerPawnLocation = PlayerController->GetPawn()->GetActorLocation();
		FVector2D PlayerIconOffset = WorldCoordsToMapCoords(PlayerPawnLocation.X, PlayerPawnLocation.Y);

		GetPlayerImage()->SetRenderTranslation(PlayerIconOffset);
		GetPlayerLookDirectionImage()->SetRenderTranslation(PlayerIconOffset);
		GetPlayerLookDirectionImage()->SetRenderTransformAngle(PlayerController->GetControlRotation().Yaw + 90);

		for (UMapPOIWidget* POIWidget : POIWidgets)
		{
			POIWidget->SetRenderTranslation(WorldCoordsToMapCoords(POIWidget->GetPOILocation().X, POIWidget->GetPOILocation().Y));
			POIWidget->SetRenderScale(FVector2D(Zoom, Zoom));
			POIWidget->SetRenderTransformAngle(POIWidget->GetPOIRotationYaw());
		}
	}
}

FReply UMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::Handled().NativeReply;
	}

	return UWidgetBlueprintLibrary::Unhandled().NativeReply;
}

FReply UMapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
		
		SetOffset(Offset + InMouseEvent.GetCursorDelta() / ViewportScale);

		return UWidgetBlueprintLibrary::Handled().NativeReply;
	}

	return UWidgetBlueprintLibrary::Unhandled().NativeReply;
}

void UMapWidget::CreatePOIWidgets()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMapPOIActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AMapPOIActor* MapPOIActor = (AMapPOIActor*)Actor;

		UMapPOIWidget* POIWidget = CreateWidget<UMapPOIWidget>(this, MapPOIActor->POIWidgetClass.Get());
		POIWidget->SetPOIActor(MapPOIActor);
		if (MapPOIActor->MatchBoxSize)
		{
			POIWidget->GetImageWidget()->SetBrushSize(WorldCoordsToMapCoords(MapPOIActor->GetActorScale3D().X, MapPOIActor->GetActorScale3D().Y) / Zoom * 200);
		}

		GetPOIWidgetsOverlay()->AddChildToOverlay(POIWidget);
		UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(POIWidget);
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

		POIWidgets.Add(POIWidget);
	}
}

void UMapWidget::SetMaterial(UMaterialInterface* MaterialInterface, FVector2D TheTextureSize)
{
	GetMapImage()->SetBrushFromMaterial(MaterialInterface);
}

void UMapWidget::SetZoom(float TheZoom)
{
	FVector2D NewOffset = Offset * TheZoom / Zoom;
	Zoom = TheZoom;
	GetMapImage()->SetRenderScale(FVector2D(Zoom, Zoom));
	SetOffset(NewOffset);
}

void UMapWidget::SetOffset(FVector2D TheOffset)
{
	FVector2D ScaledTextureSize = TextureSize * Zoom;
	if (TheOffset.X > ScaledTextureSize.X / 2)
	{
		TheOffset.X = ScaledTextureSize.X / 2;
	}
	if (TheOffset.Y > ScaledTextureSize.Y / 2)
	{
		TheOffset.Y = ScaledTextureSize.Y / 2;
	}
	if (TheOffset.X < ScaledTextureSize.X / -2)
	{
		TheOffset.X = ScaledTextureSize.X / -2;
	}
	if (TheOffset.Y < ScaledTextureSize.Y / -2)
	{
		TheOffset.Y = ScaledTextureSize.Y / -2;
	}
	Offset = TheOffset;

	SetRenderTranslation(Offset);
}

void UMapWidget::SetIconsScale(float TheScale)
{
	FVector2D ScaleVector = FVector2D(TheScale, TheScale);
	GetPlayerImage()->SetRenderScale(ScaleVector);
	GetPlayerLookDirectionImage()->SetRenderScale(ScaleVector);
}

void UMapWidget::ZoomIn()
{
	if (Zoom < 8)
	{
		SetZoom(Zoom * 1.5);
	}
}

void UMapWidget::ZoomOut()
{
	if (Zoom > 0.25)
	{
		SetZoom(Zoom / 1.5);
	}
}

FVector2D UMapWidget::WorldCoordsToMapCoords(float X, float Y)
{
	return (FVector2D(X, Y)) * Zoom
		/ CmPerPixel
		+ TextureOffset * Zoom;
}

void UMapWidget::OnFloatSettingUpdated(FString Key, float Value)
{
	if (Key.Equals(ABasePlayerController::MAP_ICONS_SCALE_SETTING_KEY))
	{
		SetIconsScale(Value);
	}
}
