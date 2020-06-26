// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeadBodyInventoryUI.generated.h"

class ADeadBodyBackpack;
class UInventoryComponent;
class UInventoryUI;
class UItemWidget;

/**
 * 
 */
UCLASS()
class STALKERMP_API UDeadBodyInventoryUI : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UInventoryComponent* InventoryComponent;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventoryBackpackWidget* GetInventoryBackpackWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventorySlotWidget* GetArmorInventorySlotWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventorySlotWidget* GetWeapon1InventorySlotWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UInventorySlotWidget* GetWeapon2InventorySlotWidget();


public:

	UFUNCTION()
		void SetInventoryComponent(UInventoryComponent* TheInventoryComponent) { InventoryComponent = TheInventoryComponent; }

	UFUNCTION()
		void SetDeadBodyBackpack(ADeadBodyBackpack* DeadBodyBackpack);

	UFUNCTION()
		void OnItemGone(UItemWidget* ItemWidget);


};
