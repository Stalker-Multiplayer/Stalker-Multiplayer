// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class STALKERMP_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UTextBlock* GetTooltipTitleBox();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UTextBlock* GetTooltipDetailsBox();


public:

	UFUNCTION()
		void SetTexts(FText Title, FText Details);


};
