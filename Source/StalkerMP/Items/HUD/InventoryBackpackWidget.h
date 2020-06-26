// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBackpackWidget.generated.h"

class UItemWidget;
class UInventoryComponent;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class STALKERMP_API UInventoryBackpackWidget : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UUserWidget*** CellWidgetsArray;

	UPROPERTY()
		UInventoryComponent* InventoryComponent;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> CellWidgetClass;




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
		void SetBackpackSize(FIntPoint InventorySize);

	UFUNCTION()
		void AddItemWidget(UItemWidget* ItemWidget, FIntPoint Position);

};
