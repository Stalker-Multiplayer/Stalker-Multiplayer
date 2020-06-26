// Fill out your copyright notice in the Description page of Project Settings.

#include "MinimapWidget.h"

#include "StalkerMP/DataClasses/BaseLevelDataActor.h"
#include "StalkerMP/GameModes/BasePlayerController.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"


void UMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseLevelDataActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ABaseLevelDataActor* LevelDataActor = (ABaseLevelDataActor*) FoundActors[0];

		SetMaterial(LevelDataActor->MapMaterial);

		CmPerPixel = LevelDataActor->MapCmPerPixel;
		TextureSize = LevelDataActor->MapTextureSize;
		TextureOffset = LevelDataActor->MapTextureOffset;

		Size = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(
			ESettingsType::General,
			ABasePlayerController::MINIMAP_SIZE_SETTING_KEY,
			ABasePlayerController::DEFAULT_MINIMAP_SIZE);
		SetSize(Size);

		Zoom = GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
			ESettingsType::General,
			ABasePlayerController::MINIMAP_ZOOM_SETTING_KEY,
			ABasePlayerController::DEFAULT_MINIMAP_ZOOM);
		SetZoom(Zoom);

		SetIconsScale(GetGameInstance<UStalkerMPGameInstance>()->GetFloatSettingValue(
			ESettingsType::General,
			ABasePlayerController::MINIMAP_ICONS_SCALE_SETTING_KEY,
			ABasePlayerController::DEFAULT_MINIMAP_ICONS_SIZE));
	}

	OnFloatSettingValueUpdatedDelegate.BindUFunction(this, "OnFloatSettingUpdated");
	UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.AddUnique(OnFloatSettingValueUpdatedDelegate);
	OnIntSettingValueUpdatedDelegate.BindUFunction(this, "OnIntSettingUpdated");
	UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.AddUnique(OnIntSettingValueUpdatedDelegate);
}

void UMinimapWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UStalkerMPGameInstance::OnFloatSettingValueUpdatedDelegate.Remove(OnFloatSettingValueUpdatedDelegate);
	UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.Remove(OnIntSettingValueUpdatedDelegate);
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (PlayerController->GetPawn())
	{
		FVector PlayerPawnLocation = PlayerController->GetPawn()->GetActorLocation();

		GetMinimapImage()->SetRenderTranslation(WorldPosToMinimapOffset(PlayerPawnLocation.X, PlayerPawnLocation.Y));
		GetPlayerLookDirectionImage()->SetRenderTransformAngle(PlayerController->GetControlRotation().Yaw + 90);
	}
}

FVector2D UMinimapWidget::WorldPosToMinimapOffset(float PosX, float PosY)
{
	return ((FVector2D(PosX, PosY)) * Zoom * Size
		/ CmPerPixel
		+ TextureOffset * Zoom * Size)
		/ TextureSize
		* -1;
}

void UMinimapWidget::SetMaterial(UMaterialInterface* MaterialInterface)
{
	GetMinimapImage()->SetBrushFromMaterial(MaterialInterface);
}

void UMinimapWidget::SetSize(float TheSize)
{
	Size = TheSize;
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetSize(FVector2D(Size, Size));
	}
}

void UMinimapWidget::SetZoom(float TheZoom)
{
	Zoom = TheZoom;
	GetMinimapImage()->SetRenderScale(FVector2D(Zoom, Zoom));
}

void UMinimapWidget::SetIconsScale(float TheScale)
{
	FVector2D ScaleVector = FVector2D(TheScale, TheScale);
	GetPlayerImage()->SetRenderScale(ScaleVector);
	GetPlayerLookDirectionImage()->SetRenderScale(ScaleVector);
}

void UMinimapWidget::OnFloatSettingUpdated(FString Key, float Value)
{
	if (Key.Equals(ABasePlayerController::MINIMAP_ZOOM_SETTING_KEY))
	{
		Zoom = Value;
		SetZoom(Zoom);
	}
	else if (Key.Equals(ABasePlayerController::MINIMAP_ICONS_SCALE_SETTING_KEY))
	{
		SetIconsScale(Value);
	}
}

void UMinimapWidget::OnIntSettingUpdated(FString Key, int Value)
{
	if (Key.Equals(ABasePlayerController::MINIMAP_SIZE_SETTING_KEY))
	{
		Size = Value;
		SetSize(Size);
	}
}
