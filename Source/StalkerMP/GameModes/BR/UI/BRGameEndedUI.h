// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "BRGameEndedUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UBRGameEndedUI : public UHideableWidget
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void Close();


public:

};
