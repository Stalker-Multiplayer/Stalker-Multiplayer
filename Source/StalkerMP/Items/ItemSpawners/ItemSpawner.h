// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

class ABaseItem;
class AItemSpawnRegion;

class UBoxComponent;

USTRUCT(BlueprintType)
struct FSpawnableItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ABaseItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ItemWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int SpawnChance = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Unique = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<class ABaseItem>> RequiredSatteliteItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<TSubclassOf<class ABaseItem>> OptionalSatelliteItems;

};

USTRUCT(BlueprintType)
struct FRequiredItemToSpawnList
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		TArray<FSpawnableItemData> ItemsList;

};

UCLASS()
class STALKERMP_API AItemSpawner : public AActor
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

private:




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	// Sets default values for this actor's properties
	AItemSpawner();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		TArray<AItemSpawnRegion*> ItemSpawnRegionsInside;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FRequiredItemToSpawnList> RequiredItemsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FSpawnableItemData> OptionalItemsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int TotalItemsWeight = 0;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		ABaseItem* SpawnItem(AItemSpawnRegion* ItemSpawnRegion, FSpawnableItemData SpawnableItemData);

	UFUNCTION()
		ABaseItem* SpawnItemByClass(AItemSpawnRegion* ItemSpawnRegion, TSubclassOf<class ABaseItem> ItemClass);


public:

	UFUNCTION()
		void SpawnItems();

	UFUNCTION()
		int GetTotalItemsWeight() { return TotalItemsWeight; }


	
};
