#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRegion.generated.h"

class UBoxComponent;

UCLASS()
class STALKERMP_API AItemSpawnRegion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnRegion();

protected:




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* SpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool SpawnedItemsFollowRotation = false;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		bool IsSpawnedItemsFollowRotation() { return SpawnedItemsFollowRotation; }

	UFUNCTION()
		FVector GetSpawnLocation();


};
