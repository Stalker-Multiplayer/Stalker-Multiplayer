// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBushActor.generated.h"

class UInstancedStaticMeshComponent;
class USoundBase;

USTRUCT()
struct FBushArrayWrapper
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TArray<UPrimitiveComponent*> Array;

};

/**
 *
 */
UCLASS()
class STALKERMP_API ABaseBushActor : public AActor
{
	GENERATED_BODY()



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	ABaseBushActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		TMap<AActor*, FBushArrayWrapper> Map_TouchedBushes;

	UPROPERTY()
		FTimerHandle PlaySoundsTimerHandle;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USoundBase* BushTouchSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PlaySoundDelay = 0.25;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float OtherActorSpeedForDefaultVolume = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<AActor*> LevelActors;





// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void PlaySounds();

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
