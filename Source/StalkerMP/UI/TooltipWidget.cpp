// Fill out your copyright notice in the Description page of Project Settings.

#include "TooltipWidget.h"

#include "Components/TextBlock.h"


void UTooltipWidget::SetTexts(FText Title, FText Details)
{
	GetTooltipTitleBox()->SetText(Title);
	GetTooltipDetailsBox()->SetText(Details);
}
