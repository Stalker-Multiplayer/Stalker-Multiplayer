// Fill out your copyright notice in the Description page of Project Settings.

#include "MapPOIWidget.h"

#include "StalkerMP/UI/Map/MapPOIActor.h"
#include "StalkerMP/UI/TooltipWidget.h"

#include "Components/Image.h"

void UMapPOIWidget::SetPOIActor(AMapPOIActor* TheMapPOIActor)
{
	MapPOIActor = TheMapPOIActor;

	UTooltipWidget* TooltipWidget = CreateWidget<UTooltipWidget>(GetOwningPlayer(), MapPOIActor->TooltipWidgetClass.Get());
	TooltipWidget->SetTexts(MapPOIActor->TooltipTitle, MapPOIActor->TooltipText);

	GetImageWidget()->SetToolTip(TooltipWidget);
}

FVector UMapPOIWidget::GetPOILocation()
{
	return MapPOIActor->GetActorLocation();
}

float UMapPOIWidget::GetPOIRotationYaw()
{
	if (MapPOIActor->RotateAccordingToBox)
	{
		return MapPOIActor->GetActorRotation().Yaw;
	}
	else
	{
		return 0;
	}
}

FVector UMapPOIWidget::GetPOISize()
{
	return MapPOIActor->GetScaledBoxSize();
}
