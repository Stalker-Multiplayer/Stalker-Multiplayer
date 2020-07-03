// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseLevelDataActor.generated.h"

class USphereComponent;
class UMaterialInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGamemodeLevelLoadedDelegate);

UCLASS()
class STALKERMP_API ABaseLevelDataActor : public AActor
{
	GENERATED_BODY()


// ------------------------------------------------------------------------- Delegates --------------------------------------------------------------------------

public:

	static FGamemodeLevelLoadedDelegate GamemodeLevelLoadedDelegate;



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	ABaseLevelDataActor();
	virtual void BeginPlay() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* EmptySphere;


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		UMaterialInterface* MapMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		float MapCmPerPixel = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		float MapDefaultZoom = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		FVector2D MapTextureSize = FVector2D(128, 128);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
		FVector2D MapTextureOffset = FVector2D(0, 0);


};
