// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class UPickableItemsWidget;
class UInventorySlotWidget;
class UInventoryBackpackWidget;
class UInventoryComponent;
class UDeadBodyInventoryUI;
class UCanvasPanel;

UCLASS()
class STALKERMP_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
	


// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

	void NativeConstruct() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UInventoryComponent* InventoryComponent;

	UPROPERTY()
		ADeadBodyBackpack* DeadBodyBackpack;


public:

	static const int CELL_SIZE_PIXELS = 80;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UCanvasPanel* GetCanvasPanel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UPickableItemsWidget* GetPickableItemsWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventoryBackpackWidget* GetInventoryBackpackWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventorySlotWidget* GetArmorInventorySlotWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventorySlotWidget* GetWeapon1InventorySlotWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventorySlotWidget* GetWeapon2InventorySlotWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UDeadBodyInventoryUI* GetDeadBodyInventoryUI();


public:

	UFUNCTION()
		void SetInventoryComponent(UInventoryComponent* TheInventoryComponent) { InventoryComponent = TheInventoryComponent; }

	UFUNCTION()
		void OnPickableItemAppeared(UItemWidget* ItemWidget);

	UFUNCTION()
		void OnPickableItemGone(UItemWidget* ItemWidget);

	UFUNCTION()
		void OnDeadBodyItemGone(UItemWidget* ItemWidget);

	UFUNCTION()
		void PutItemToBackpack(UItemWidget* ItemWidget, FIntPoint Position);

	UFUNCTION()
		void PutItemToArmorSlot(UItemWidget* ItemWidget);

	UFUNCTION()
		void PutItemToWeapon1Slot(UItemWidget* ItemWidget);

	UFUNCTION()
		void PutItemToWeapon2Slot(UItemWidget* ItemWidget);

	UFUNCTION()
		void SetDeadBodyBackpack(ADeadBodyBackpack* TheDeadBodyBackpack);

	UFUNCTION()
		bool IsDeadBodyInventoryUIVisible();


};
