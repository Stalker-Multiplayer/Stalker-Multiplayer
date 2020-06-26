// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "BaseGameSession.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API ABaseGameSession : public AGameSession
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	virtual void RegisterServer() override;
	virtual void RegisterServerFailed() override;


};
