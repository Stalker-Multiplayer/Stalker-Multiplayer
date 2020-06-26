// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

class UCanvasPanel;
class UMinimapWidget;
class UMapWidget;

/**
 * 
 */
UCLASS()
class STALKERMP_API UGameUI : public UUserWidget
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		FTimerHandle KilledPlayerVisibleTimerHandler;

	UPROPERTY()
		UMinimapWidget* MinimapWidget;

	UPROPERTY()
		UMapWidget* MapWidget;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int KilledPlayerVisibilityTime = 5;



// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		UCanvasPanel* GetRootCanvas();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void OnPlayerDiedBP(const FString &PlayerName, const FString &KilledName, bool KilledByMe, bool MeDied);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
		void HideDiedPlayer();


public:

	UFUNCTION(BlueprintCallable)
		void SetPlayerController(ABasePlayerController* ThePlayerController);

	UFUNCTION()
		void MapZoomIn();

	UFUNCTION()
		void MapZoomOut();

	UFUNCTION()
		bool IsMapVisible();

	UFUNCTION()
		void ShowMap();

	UFUNCTION()
		void HideMap();

	UFUNCTION()
		void OnPlayerDied(FString PlayerName, FString KilledName, bool KilledByMe, bool MeDied);


};
