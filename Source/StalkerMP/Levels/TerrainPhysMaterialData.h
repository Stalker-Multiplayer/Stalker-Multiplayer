// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainPhysMaterialData.generated.h"

class USphereComponent;
class UTextureRenderTarget2D;

UCLASS()
class STALKERMP_API ATerrainPhysMaterialData : public AActor
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	ATerrainPhysMaterialData();
	//virtual void Tick(float DeltaTime) override;



// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UTextureRenderTarget2D* TerrainMaskRenderTarget;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* EmptySphere;

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




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION(BlueprintCallable)
		void SetTerrainMaskRenderTarget(UTextureRenderTarget2D* TheTerrainMaskRenderTarget) { TerrainMaskRenderTarget = TheTerrainMaskRenderTarget; }


public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TEnumAsByte<EPhysicalSurface> GetSurfaceAt(FHitResult HitResult);

};
