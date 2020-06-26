// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BRGameEndedUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UBRGameEndedUI : public UUserWidget
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void Close();


public:

};
