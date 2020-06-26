// Fill out your copyright notice in the Description page of Project Settings.

#include "BRMinimapWidget.h"

#include "StalkerMP/GameModes/BR/Actors/BRLevelDataActor.h"
#include "StalkerMP/GameModes/BR/Actors/BRZoneActor.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"


void UBRMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABRLevelDataActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ABRLevelDataActor* LevelDataActor = (ABRLevelDataActor*)FoundActors[0];

		ZoneActor = LevelDataActor->ZoneActor;
		MapMaterialParameterCollection = LevelDataActor->MapMaterialParameterCollection;
	}
}

void UBRMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector ZoneActorLocation = ZoneActor->GetActorLocation();

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MapMaterialParameterCollection, "ZoneX", ZoneActorLocation.X / TextureSize.X / CmPerPixel + 0.5);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MapMaterialParameterCollection, "ZoneY", ZoneActorLocation.Y / TextureSize.Y / CmPerPixel + 0.5);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MapMaterialParameterCollection, "ZoneSize", ZoneActor->GetActorScale3D().X * 50 / TextureSize.X / CmPerPixel);
}
