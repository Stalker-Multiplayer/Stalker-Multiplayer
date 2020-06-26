// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDragWidget.generated.h"

class UTexture;

UCLASS()
class STALKERMP_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()
	
	


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UTexture2D* IconImageTexture;

	UPROPERTY()
		FVector2D IconImageSize;


public:

	UPROPERTY()
		FVector2D DragOffset;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintPure)
		UTexture2D* GetIconImageTexture() { return IconImageTexture; }

	UFUNCTION(BlueprintPure)
		FVector2D GetIconImageSize() { return IconImageSize; }

public:

	UFUNCTION(BlueprintCallable)
		void SetIconImage(UTexture2D* Texture, FVector2D ImageSize) { IconImageTexture = Texture; IconImageSize = ImageSize; }
	
};
