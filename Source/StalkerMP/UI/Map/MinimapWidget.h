// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "MinimapWidget.generated.h"

class ABasePlayerController;
class ABaseLevelDataActor;

class UMaterialInterface;
class UImage;

/**
 * 
 */
UCLASS()
class STALKERMP_API UMinimapWidget : public UHideableWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Override --------------------------------------------------------------------------

protected:

	virtual void OnIntSettingUpdated(FString Key, int Value) override;


public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnFloatSettingValueUpdatedDelegate;
	FScriptDelegate OnIntSettingValueUpdatedDelegate;


protected:

	UPROPERTY()
		float Zoom = 1;

	UPROPERTY()
		float Size = 250;

	UPROPERTY()
		float CmPerPixel = 100;

	UPROPERTY()
		FVector2D TextureSize = FVector2D(128, 128);

	UPROPERTY()
		FVector2D TextureOffset = FVector2D(0, 0);

	UPROPERTY()
		ABasePlayerController* PlayerController;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnFloatSettingUpdated(FString Key, float Value);


protected:

	UFUNCTION()
		virtual void SetMaterial(UMaterialInterface* MaterialInterface);

	UFUNCTION()
		virtual void SetSize(float TheSize);

	UFUNCTION()
		virtual void SetZoom(float TheZoom);

	UFUNCTION()
		virtual void SetIconsScale(float TheScale);

	UFUNCTION()
		FVector2D WorldPosToMinimapOffset(float PosX, float PosY);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetMinimapImage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetPlayerImage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UImage* GetPlayerLookDirectionImage();


public:

	UFUNCTION(BlueprintCallable)
		void SetPlayerController(ABasePlayerController* ThePlayerController) { PlayerController = ThePlayerController; }


};
