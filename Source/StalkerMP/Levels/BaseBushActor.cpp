// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBushActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ABaseBushActor::ABaseBushActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABaseBushActor::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && BushTouchSound)
	{
		for (UActorComponent* Child : GetComponents())
		{
			if (UStaticMeshComponent* StaticMesh = dynamic_cast<UStaticMeshComponent*>(Child)) {
				StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseBushActor::OnBeginOverlap);
				StaticMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseBushActor::OnEndOverlap);
			}
			else if (UInstancedStaticMeshComponent* InstancedMesh = dynamic_cast<UInstancedStaticMeshComponent*>(Child)) {
				InstancedMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseBushActor::OnBeginOverlap);
				InstancedMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseBushActor::OnEndOverlap);
			}
		}

		for (AActor* LevelActor : LevelActors)
		{
			if (AStaticMeshActor* StaticMesh = dynamic_cast<AStaticMeshActor*>(LevelActor)) {
				StaticMesh->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseBushActor::OnBeginOverlap);
				StaticMesh->GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &ABaseBushActor::OnEndOverlap);
			}
			else
			{
				TArray<USceneComponent*> ChildComponents;
				LevelActor->GetRootComponent()->GetChildrenComponents(true, ChildComponents);
				for (USceneComponent* ChildComponent : ChildComponents)
				{
					if (UStaticMeshComponent* ChildStaticMesh = dynamic_cast<UStaticMeshComponent*>(ChildComponent)) {
						ChildStaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseBushActor::OnBeginOverlap);
						ChildStaticMesh->OnComponentEndOverlap.AddDynamic(this, &ABaseBushActor::OnEndOverlap);
					}
				}
			}
		}

		GetWorld()->GetTimerManager().SetTimer(PlaySoundsTimerHandle, this, &ABaseBushActor::PlaySounds, PlaySoundDelay, true);
	}
}

void ABaseBushActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		Map_TouchedBushes.Empty();

		for (UActorComponent* Child : GetComponents())
		{
			if (UStaticMeshComponent* StaticMesh = dynamic_cast<UStaticMeshComponent*>(Child)) {
				StaticMesh->OnComponentBeginOverlap.RemoveAll(this);
				StaticMesh->OnComponentEndOverlap.RemoveAll(this);
			}
			else if (UInstancedStaticMeshComponent* InstancedMesh = dynamic_cast<UInstancedStaticMeshComponent*>(Child)) {
				InstancedMesh->OnComponentBeginOverlap.RemoveAll(this);
				InstancedMesh->OnComponentEndOverlap.RemoveAll(this);
			}
		}

		for (AActor* LevelActor : LevelActors)
		{
			if (AStaticMeshActor* StaticMesh = dynamic_cast<AStaticMeshActor*>(LevelActor)) {
				StaticMesh->GetStaticMeshComponent()->OnComponentBeginOverlap.RemoveAll(this);
				StaticMesh->GetStaticMeshComponent()->OnComponentEndOverlap.RemoveAll(this);
			}
			else
			{
				TArray<USceneComponent*> ChildComponents;
				LevelActor->GetRootComponent()->GetChildrenComponents(true, ChildComponents);
				for (USceneComponent* ChildComponent : ChildComponents)
				{
					if (UStaticMeshComponent* ChildStaticMesh = dynamic_cast<UStaticMeshComponent*>(ChildComponent)) {
						ChildStaticMesh->OnComponentBeginOverlap.RemoveAll(this);
						ChildStaticMesh->OnComponentEndOverlap.RemoveAll(this);
					}
				}
			}
		}
	}
}

void ABaseBushActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Map_TouchedBushes.Contains(OtherActor))
	{
		Map_TouchedBushes.Add(OtherActor, FBushArrayWrapper());
	}

	Map_TouchedBushes[OtherActor].Array.AddUnique(OverlappedComponent);
}

void ABaseBushActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Map_TouchedBushes[OtherActor].Array.Remove(OverlappedComponent);
}

void ABaseBushActor::PlaySounds()
{
	for (const TPair<AActor*, FBushArrayWrapper>& pair : Map_TouchedBushes)
	{
		if (pair.Value.Array.Num() > 0)
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BushTouchSound, pair.Key->GetActorLocation(), FRotator(0, 0, 0),
				pair.Key->GetVelocity().Size() / OtherActorSpeedForDefaultVolume);
		}
	}
}
