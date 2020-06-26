// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

class ABaseStackableItem;
class UItemDragWidget;
class AItemActor;

UCLASS()
class STALKERMP_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		ABaseItem* Item;

	UPROPERTY()
		ABaseStackableItem* StackableItem;


protected:

	UPROPERTY(BlueprintReadWrite)
		FVector2D DragOffset = FVector2D(0, 0);

	UPROPERTY(BlueprintReadOnly)
		FVector2D IconImageSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class UItemDragWidget> ItemDragWidgetClass;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void SetIconImage(UTexture2D* ImageTexture, FVector2D ImageSize);

	UFUNCTION(BlueprintPure, BlueprintCosmetic)
		int GetStackSize();


public:

	UFUNCTION()
		void FixIconSize();

	UFUNCTION()
		void SetItem(ABaseItem* TheItem);

	template< class T >
	T* GetItem() const
	{
		return Cast<T>(Item);
	}

	UFUNCTION()
		ABaseItem* GetItem() { return Item; }

};
