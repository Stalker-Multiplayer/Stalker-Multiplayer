// Fill out your copyright notice in the Description page of Project Settings.


#include "BRSafeZoneActor.h"

#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABRSafeZoneActor::ABRSafeZoneActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	SetReplicates(true);
	NetCullDistanceSquared = 40000000000.0;

	StaticMeshOutside = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshOutside"));
	StaticMeshOutside->SetCollisionProfileName("NoCollision");
	StaticMeshOutside->SetEnableGravity(false);
	StaticMeshOutside->bApplyImpulseOnDamage = false;
	StaticMeshOutside->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	StaticMeshOutside->SetCanEverAffectNavigation(false);
	StaticMeshOutside->bCastStaticShadow = false;
	StaticMeshOutside->SetCastShadow(false);
	RootComponent = StaticMeshOutside;

	StaticMeshInside = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshInside"));
	StaticMeshInside->SetCollisionProfileName("NoCollision");
	StaticMeshInside->SetEnableGravity(false);
	StaticMeshInside->bApplyImpulseOnDamage = false;
	StaticMeshInside->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	StaticMeshInside->SetCanEverAffectNavigation(false);
	StaticMeshInside->bCastStaticShadow = false;
	StaticMeshInside->SetCastShadow(false);
	StaticMeshInside->SetupAttachment(StaticMeshOutside);
}

void ABRSafeZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABRSafeZoneActor, ChangingSizeRichCurve);
}

void ABRSafeZoneActor::BeginPlay()
{
	Super::BeginPlay();

	ChangingSizeTimeline = NewObject<UTimelineComponent>(this, FName("SizeTimeline"));
	ChangingSizeTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(ChangingSizeTimeline); // Add to array so it gets saved
	ChangingSizeTimeline->SetNetAddressable(); // This component has a stable name that can be referenced for replication
	ChangingSizeTimeline->SetIsReplicated(true);
	ChangingSizeTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	ChangingSizeTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
	ChangingSizeTimeline->SetLooping(false);
	ChangingSizeTimeline->RegisterComponent();

	ChangingSizeCurve = NewObject<UCurveFloat>();
	FOnTimelineFloat onSizeChangeCallback;
	onSizeChangeCallback.BindUFunction(this, FName{ TEXT("UpdateSize") });
	ChangingSizeTimeline->AddInterpFloat(ChangingSizeCurve, onSizeChangeCallback);

	ChangingSizeCurve->FloatCurve = ChangingSizeRichCurve;
	ChangingSizeTimeline->SetTimelineLength(TimelineLength);
}

void ABRSafeZoneActor::OnRep_ChangingSizeRichCurve()
{
	if (ChangingSizeTimeline)
	{
		ChangingSizeCurve->FloatCurve = ChangingSizeRichCurve;
	}
}

void ABRSafeZoneActor::OnRep_TimelineLength()
{
	if (ChangingSizeTimeline)
	{
		ChangingSizeTimeline->SetTimelineLength(TimelineLength);
	}
}

void ABRSafeZoneActor::UpdateSize(float Size)
{
	SetActorScale3D(FVector(Size, Size, Size));
	if (HasAuthority() && TimelineLength > 0 && ChangingSizeTimeline->GetPlaybackPosition() >= TimelineLength)
	{
		Destroy();
	}
}

void ABRSafeZoneActor::SetParams(float FullSize, float FinalSize, int ExpandDuration, int ShrinkDuration, bool SpawnArtefact)
{
	TimelineLength = ExpandDuration + ShrinkDuration + 1;

	ChangingSizeRichCurve = FRichCurve();
	ChangingSizeRichCurve.AddKey(0, 0);
	ChangingSizeRichCurve.AddKey(ExpandDuration, FullSize);
	if (FinalSize > 0)
	{
		ChangingSizeRichCurve.AddKey(TimelineLength - 0.5, FinalSize);
	}
	ChangingSizeRichCurve.AddKey(TimelineLength, 0);

	ChangingSizeTimeline->SetTimelineLength(TimelineLength);
	ChangingSizeCurve->FloatCurve = ChangingSizeRichCurve;
	ChangingSizeTimeline->Play();
}
