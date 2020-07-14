// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StalkerMP/UI/HideableWidget.h"

#include "DMGameEndedUI.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API UDMGameEndedUI : public UHideableWidget
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void Close();


public:

};
