// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"

#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UInventoryComponent;
class UItemWidget;
class UCanvasPanel;


/**
 *
 */
UCLASS()
class STALKERMP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY()
		UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ESlotType SlotType = ESlotType::ST_None;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


protected:

	// Should be implemented in Blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UCanvasPanel* GetCanvasPanel();


public:

	UFUNCTION()
		void SetInventoryComponent(UInventoryComponent* TheInventoryComponent) { InventoryComponent = TheInventoryComponent; }

	UFUNCTION()
		void AddItemWidget(UItemWidget* ItemWidget);

};
