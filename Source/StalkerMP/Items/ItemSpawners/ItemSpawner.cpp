// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSpawner.h"

#include "StalkerMP/Items/ItemSpawners/ItemSpawnRegion.h"
#include "StalkerMP/Items/ArmorItem.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"


// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//bNetLoadOnClient = false;
	SetActorHiddenInGame(true);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetMobility(EComponentMobility::Static);
	SpawnBox->SetBoxExtent(FVector(64, 64, 32), false);
	SpawnBox->SetCollisionProfileName(TEXT("NoCollision"));
	SpawnBox->CanCharacterStepUpOn = ECB_No;
	SpawnBox->SetGenerateOverlapEvents(false);
	SpawnBox->SetCanEverAffectNavigation(false);
	SpawnBox->SetEnableGravity(false);
	SpawnBox->bApplyImpulseOnDamage = false;
	SpawnBox->SetCanEverAffectNavigation(false);
	SpawnBox->AreaClass = nullptr;

#if WITH_EDITORONLY_DATA
	SpawnBox->bVisualizeComponent = true;
#endif

	RootComponent = SpawnBox;
}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// Getting all ItemSpawnerRegions inside this ItemSpawner
		for (TActorIterator<AItemSpawnRegion> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AItemSpawnRegion* ItemSpawnRegion = *ActorItr;

			if (UKismetMathLibrary::IsPointInBoxWithTransform(ItemSpawnRegion->GetActorLocation(), GetTransform(), SpawnBox->GetUnscaledBoxExtent()))
			{
				ItemSpawnRegionsInside.Add(ItemSpawnRegion);
			}
		}
	}
}

void AItemSpawner::SpawnItems()
{
	int ItemsWeightLeft = TotalItemsWeight;
	TArray<AItemSpawnRegion*> EmptyItemSpawnRegions = ItemSpawnRegionsInside;

	if (RequiredItemsToSpawn.Num() > 0)
	{
		for (FRequiredItemToSpawnList RequiredItemToSpawnList : RequiredItemsToSpawn)
		{
			TArray<FSpawnableItemData> ItemsList = RequiredItemToSpawnList.ItemsList;

			if (ItemsList.Num() <= 0)
			{
				continue;
			}

			int TotalItemsSpawnChance = 0;
			for (FSpawnableItemData SpawnableItemData : ItemsList)
			{
				TotalItemsSpawnChance += SpawnableItemData.SpawnChance;
			}

			AItemSpawnRegion* ItemSpawnRegion = EmptyItemSpawnRegions[UKismetMathLibrary::RandomInteger(EmptyItemSpawnRegions.Num())];
			int ItemNumber = UKismetMathLibrary::RandomInteger(TotalItemsSpawnChance);

			int CurrentItemsSpawnChance = 0;
			for (FSpawnableItemData SpawnableItemData : ItemsList)
			{
				CurrentItemsSpawnChance += SpawnableItemData.SpawnChance;
				if (ItemNumber < CurrentItemsSpawnChance)
				{
					if (ABaseItem* SpawnedItem = SpawnItem(ItemSpawnRegion, SpawnableItemData))
					{
						ItemsWeightLeft -= SpawnableItemData.ItemWeight;
						EmptyItemSpawnRegions.Remove(ItemSpawnRegion);
						break;
					}
				}
			}

			if (EmptyItemSpawnRegions.Num() <= 0)
			{
				break;
			}
		}
	}

	if (OptionalItemsToSpawn.Num() > 0)
	{
		int TotalItemsSpawnChance = 0;
		for (FSpawnableItemData SpawnableItemData : OptionalItemsToSpawn)
		{
			TotalItemsSpawnChance += SpawnableItemData.SpawnChance;
		}

		while (EmptyItemSpawnRegions.Num() > 0 && ItemsWeightLeft > 0)
		{
			AItemSpawnRegion* ItemSpawnRegion = EmptyItemSpawnRegions[UKismetMathLibrary::RandomInteger(EmptyItemSpawnRegions.Num())];
			int ItemNumber = UKismetMathLibrary::RandomInteger(TotalItemsSpawnChance);

			int CurrentItemsSpawnChance = 0;
			for (FSpawnableItemData SpawnableItemData : OptionalItemsToSpawn)
			{
				CurrentItemsSpawnChance += SpawnableItemData.SpawnChance;
				if (ItemNumber < CurrentItemsSpawnChance)
				{
					if (ABaseItem* SpawnedItem = SpawnItem(ItemSpawnRegion, SpawnableItemData))
					{
						ItemsWeightLeft -= SpawnableItemData.ItemWeight;
						EmptyItemSpawnRegions.Remove(ItemSpawnRegion);
						break;
					}
				}
			}
		}
	}
}

ABaseItem* AItemSpawner::SpawnItem(AItemSpawnRegion* ItemSpawnRegion, FSpawnableItemData SpawnableItemData)
{
	// Removing the item if it is unique and is already spawned
	if (SpawnableItemData.Unique)
	{
		TArray<AActor*> AllSuchItems;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnableItemData.ItemClass, AllSuchItems);

		if (AllSuchItems.Num() > 0)
		{
			return nullptr;
		}
	}

	for (TSubclassOf<class ABaseItem> RequiredSatteliteItem : SpawnableItemData.RequiredSatteliteItems)
	{
		SpawnItemByClass(ItemSpawnRegion, RequiredSatteliteItem);
	}

	for (TSubclassOf<class ABaseItem> OptionalSatelliteItem : SpawnableItemData.OptionalSatelliteItems)
	{
		if (UKismetMathLibrary::RandomBool())
		{
			SpawnItemByClass(ItemSpawnRegion, OptionalSatelliteItem);
		}
	}

	return SpawnItemByClass(ItemSpawnRegion, SpawnableItemData.ItemClass);
}

ABaseItem* AItemSpawner::SpawnItemByClass(AItemSpawnRegion* ItemSpawnRegion, TSubclassOf<class ABaseItem> ItemClass)
{
	FTransform SpawnTransform = FTransform();
	SpawnTransform.SetLocation(ItemSpawnRegion->GetSpawnLocation());
	FRotator SpawnRegionRotation = ItemSpawnRegion->GetActorRotation();
	if (!ItemSpawnRegion->IsSpawnedItemsFollowRotation())
	{
		SpawnRegionRotation.Yaw = UKismetMathLibrary::RandomInteger(360);
		SpawnTransform.SetRotation(SpawnRegionRotation.Quaternion());
	}
	else
	{
		SpawnTransform.SetRotation(SpawnRegionRotation.Quaternion());
	}

	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(ItemClass, SpawnTransform, SpawnInfo);

	// Adjusting the spawned item location and rotation
	SpawnedItem->AddActorLocalTransform(SpawnedItem->GetSpawnTransformOffset());

	return SpawnedItem;
}