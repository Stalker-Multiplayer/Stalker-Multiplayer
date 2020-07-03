// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseLevelDataActor.h"

#include "StalkerMP/StalkerMPGameInstance.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/BodySetup.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"

FGamemodeLevelLoadedDelegate ABaseLevelDataActor::GamemodeLevelLoadedDelegate = FGamemodeLevelLoadedDelegate();

// Sets default values
ABaseLevelDataActor::ABaseLevelDataActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	EmptySphere = CreateDefaultSubobject<USphereComponent>(TEXT("EmptySphere"));
	EmptySphere->SetCollisionProfileName(TEXT("NoCollision"));
	EmptySphere->SetSphereRadius(0);
	EmptySphere->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	EmptySphere->bVisualizeComponent = true;
#endif
}

void ABaseLevelDataActor::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && TerrainMask)
	{
		MapTerrainToPhysSurfaces();
	}

	GamemodeLevelLoadedDelegate.Broadcast();
}

void ABaseLevelDataActor::MapTerrainToPhysSurfaces()
{
	TerrainMaskRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), TerrainMask->GetSizeX(), TerrainMask->GetSizeY());
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), TerrainMaskRenderTarget, Canvas, Size, Context);
	Canvas->K2_DrawTexture(TerrainMask, FVector2D(), Size, FVector2D(), FVector2D::UnitVector, FLinearColor::White, EBlendMode::BLEND_Opaque);
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
	UKismetRenderingLibrary::ReadRenderTargetPixel(GetWorld(), TerrainMaskRenderTarget, 0, 0);
}

TEnumAsByte<EPhysicalSurface> ABaseLevelDataActor::GetSurfaceAt(FHitResult HitResult)
{
	FVector2D UV;
	UGameplayStatics::FindCollisionUV(HitResult, 0, UV);
	float X = UV.X * TerrainMask->GetSizeX();
	float Y = UV.Y * TerrainMask->GetSizeY();

	if (TerrainMask)
	{
		FColor PixelColor = UKismetRenderingLibrary::ReadRenderTargetPixel(GetWorld(), TerrainMaskRenderTarget, X, Y);

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