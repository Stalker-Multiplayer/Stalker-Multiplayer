// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenuUI.generated.h"

class ABasePlayerController;

/**
 * 
 */
UCLASS()
class STALKERMP_API UBaseMenuUI : public UUserWidget
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY()
		class ABasePlayerController* PlayerController;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		virtual void SetPlayerController(ABasePlayerController* ThePlayerController) { PlayerController = ThePlayerController; }

};
