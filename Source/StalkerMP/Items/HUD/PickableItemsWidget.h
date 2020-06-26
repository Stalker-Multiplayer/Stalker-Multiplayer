// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickableItemsWidget.generated.h"

class UItemWidget;

/**
 * 
 */
UCLASS()
class STALKERMP_API UPickableItemsWidget : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UPanelWidget* GetPickableItemsBox();


public:

	UFUNCTION()
		void OnPickableItemAppeared(UItemWidget* PickableItem);

	UFUNCTION()
		void OnPickableItemGone(UItemWidget* PickableItem);


};
