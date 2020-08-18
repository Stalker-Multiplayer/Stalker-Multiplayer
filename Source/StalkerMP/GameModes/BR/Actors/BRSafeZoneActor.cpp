// Fill out your copyright notice in the Description page of Project Settings.


#include "BRSafeZoneActor.h"

#include "StalkerMP/Items/BaseItem.h"

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

	DOREPLIFETIME_CONDITION(ABRSafeZoneActor, PlaybackPosition, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ABRSafeZoneActor, Params, COND_InitialOnly);
}

void ABRSafeZoneActor::BeginPlay()
{
	Super::BeginPlay();

	ChangingSizeTimeline = NewObject<UTimelineComponent>(this, FName("SizeTimeline"));
	ChangingSizeTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(ChangingSizeTimeline); // Add to array so it gets saved
	//ChangingSizeTimeline->SetNetAddressable(); // This component has a stable name that can be referenced for replication
	//ChangingSizeTimeline->SetIsReplicated(true);
	ChangingSizeTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	ChangingSizeTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
	ChangingSizeTimeline->SetLooping(false);
	ChangingSizeTimeline->RegisterComponent();

	ChangingSizeCurve = NewObject<UCurveFloat>();
	FOnTimelineFloat onSizeChangeCallback;
	onSizeChangeCallback.BindUFunction(this, FName{ TEXT("UpdateSize") });
	ChangingSizeTimeline->AddInterpFloat(ChangingSizeCurve, onSizeChangeCallback);

	OnEverythingReplicated();
}

void ABRSafeZoneActor::OnRep_PlaybackPosition()
{
	PlaybackPositionReplicated = true;
	OnEverythingReplicated();
}

void ABRSafeZoneActor::OnRep_Params()
{
	ParamsReplicated = true;
	OnEverythingReplicated();
}

void ABRSafeZoneActor::OnEverythingReplicated()
{
	if (ChangingSizeTimeline && PlaybackPositionReplicated && ParamsReplicated)
	{
		int TimelineLength = Params.Z + Params.W;

		ChangingSizeRichCurve = FRichCurve();
		ChangingSizeRichCurve.AddKey(0, 0);
		ChangingSizeRichCurve.AddKey(Params.Z, Params.X);
		if (Params.Y > 0)
		{
			ChangingSizeRichCurve.AddKey(TimelineLength - 0.5, Params.Y);
		}
		FKeyHandle ZeroKey = ChangingSizeRichCurve.AddKey(TimelineLength, 0);
		ChangingSizeRichCurve.SetKeyInterpMode(ZeroKey, ERichCurveInterpMode::RCIM_Cubic);

		ChangingSizeTimeline->SetTimelineLength(TimelineLength);
		ChangingSizeTimeline->SetPlaybackPosition(PlaybackPosition, true, true);
		ChangingSizeCurve->FloatCurve = ChangingSizeRichCurve;

		ChangingSizeTimeline->Play();
	}
}

void ABRSafeZoneActor::UpdateSize(float Size)
{
	SetActorScale3D(FVector(Size, Size, Size));

	if (HasAuthority()) {
		PlaybackPosition = ChangingSizeTimeline->GetPlaybackPosition();

		if (ChangingSizeTimeline->GetTimelineLength() > 0 && PlaybackPosition >= ChangingSizeTimeline->GetTimelineLength())
		{
			if (SpawnArtefactOnDestroy)
			{
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(GetActorLocation() + FVector(0, 0, SpawnArtefactZOffset));

				FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				GetWorld()->SpawnActor<ABaseItem>(ArtefactItemClass, SpawnTransform, SpawnInfo);
			}
			Destroy();
		}
	}
}

void ABRSafeZoneActor::SetParams(float FullSize, float FinalSize, int ExpandDuration, int ShrinkDuration, bool SpawnArtefact)
{
	Params.X = FullSize;
	Params.Y = FinalSize;
	Params.Z = ExpandDuration;
	Params.W = ShrinkDuration;

	PlaybackPositionReplicated = true;
	ParamsReplicated = true;

	OnEverythingReplicated();

	SpawnArtefactOnDestroy = SpawnArtefact;
}
