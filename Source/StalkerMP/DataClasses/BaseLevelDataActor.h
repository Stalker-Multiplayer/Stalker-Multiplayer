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

private:

	UPROPERTY()
		UTextureRenderTarget2D* TerrainMaskRenderTarget;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		UTexture2D* TerrainMask;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		TEnumAsByte<EPhysicalSurface> PhysSurface_Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		TEnumAsByte<EPhysicalSurface> PhysSurface_GreenPositive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		TEnumAsByte<EPhysicalSurface> PhysSurface_GreenNegative;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		TEnumAsByte<EPhysicalSurface> PhysSurface_Blue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		TEnumAsByte<EPhysicalSurface> PhysSurface_Alpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
		float GreenThreshold = 0;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* EmptySphere;

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




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void MapTerrainToPhysSurfaces();


public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TEnumAsByte<EPhysicalSurface> GetSurfaceAt(FHitResult HitResult);


};
