// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainPhysMaterialData.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"

// Sets default values
ATerrainPhysMaterialData::ATerrainPhysMaterialData()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bRunOnAnyThread = false;

	EmptySphere = CreateDefaultSubobject<USphereComponent>(TEXT("EmptySphere"));
	EmptySphere->SetCollisionProfileName(TEXT("NoCollision"));
	EmptySphere->SetMobility(EComponentMobility::Static);
	EmptySphere->SetSphereRadius(0);
	EmptySphere->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	EmptySphere->bVisualizeComponent = true;
#endif
}

// For some reason this doesn't work in C++, but works in Blueprint

/*void ATerrainPhysMaterialData::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority() && !TerrainMaskRenderTarget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "1");
		TerrainMaskRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), TerrainMask->GetSizeX(), TerrainMask->GetSizeY());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "2");
		UCanvas* Canvas;
		FVector2D Size;
		FDrawToRenderTargetContext Context;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "3");
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), TerrainMaskRenderTarget, Canvas, Size, Context);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "4 - " + Size.ToString());
		Canvas->K2_DrawTexture(TerrainMask, FVector2D(), Size, FVector2D(), FVector2D::UnitVector, FLinearColor::White, EBlendMode::BLEND_Opaque);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "5");
		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "6");
		//UKismetRenderingLibrary::ReadRenderTargetUV(GetWorld(), TerrainMaskRenderTarget, 0, 0);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "7");
	}

	SetActorTickEnabled(false);
}*/

TEnumAsByte<EPhysicalSurface> ATerrainPhysMaterialData::GetSurfaceAt(FHitResult HitResult)
{
	FVector2D UV;
	UGameplayStatics::FindCollisionUV(HitResult, 0, UV);

	if (TerrainMaskRenderTarget)
	{
		FColor PixelColor = UKismetRenderingLibrary::ReadRenderTargetUV(GetWorld(), TerrainMaskRenderTarget, UV.X, UV.Y);

		if (PixelColor.R > PixelColor.G)
		{
			if (PixelColor.R > PixelColor.B)
			{
				if (PixelColor.R > PixelColor.A)
				{
					return PhysSurface_Red;
				}
				else
				{
					return PhysSurface_Alpha;
				}
			}
			else
			{
				if (PixelColor.B > PixelColor.A)
				{
					return PhysSurface_Blue;
				}
				else
				{
					return PhysSurface_Alpha;
				}
			}
		}
		else
		{
			if (PixelColor.G > PixelColor.B)
			{
				if (PixelColor.G > PixelColor.A)
				{
					if (HitResult.Location.Z >= GreenThreshold)
					{
						return PhysSurface_GreenPositive;
					}
					else
					{
						return PhysSurface_GreenNegative;
					}
				}
				else
				{
					return PhysSurface_Alpha;
				}
			}
			else
			{
				if (PixelColor.B > PixelColor.A)
				{
					return PhysSurface_Blue;
				}
				else
				{
					return PhysSurface_Alpha;
				}
			}
		}
	}
	else
	{
		return EPhysicalSurface::SurfaceType_Default;
	}
}