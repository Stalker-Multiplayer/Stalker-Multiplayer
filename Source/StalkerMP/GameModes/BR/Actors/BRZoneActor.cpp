// Fill out your copyright notice in the Description page of Project Settings.

#include "BRZoneActor.h"

#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/DamageTypes/ZoneDamageType.h"
#include "StalkerMP/StalkerMPGameInstance.h"

#include "Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Curves/CurveVector.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"


const FString ABRZoneActor::ZONE_STAGE_AMOUNT_SETTING_KEY = "BR_{Map}_ZoneStageAmount";
const FString ABRZoneActor::ZONE_STAGE_FINAL_SIZE_SETTING_KEY = "BR_{Map}_ZoneStage{StageNum}_FinalSize";
const FString ABRZoneActor::ZONE_STAGE_WAIT_TIME_SETTING_KEY = "BR_{Map}_ZoneStage{StageNum}_WaitTime";
const FString ABRZoneActor::ZONE_STAGE_SHRINK_TIME_SETTING_KEY = "BR_{Map}_ZoneStage{StageNum}_ShrinkTime";
const FString ABRZoneActor::ZONE_STAGE_DAMAGE_SETTING_KEY = "BR_{Map}_ZoneStage{StageNum}_Damage";

// Sets default values
ABRZoneActor::ABRZoneActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	NetCullDistanceSquared = 40000000000.0;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionProfileName("Zone");
	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->bApplyImpulseOnDamage = false;
	StaticMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	StaticMeshComponent->SetCanEverAffectNavigation(false);
	StaticMeshComponent->bCastStaticShadow = false;
	StaticMeshComponent->SetCastShadow(false);
	RootComponent = StaticMeshComponent;
}

void ABRZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABRZoneActor, ZoneTimeLineLength);
	DOREPLIFETIME(ABRZoneActor, ZoneSizeRichCurve);
	DOREPLIFETIME(ABRZoneActor, ZoneLocationRichCurveX);
	DOREPLIFETIME(ABRZoneActor, ZoneLocationRichCurveY);
	DOREPLIFETIME(ABRZoneActor, ZoneLocationRichCurveZ);
}

void ABRZoneActor::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
	}

	ZoneTimeline = NewObject<UTimelineComponent>(this, FName("ZoneTimeline"));
	ZoneTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(ZoneTimeline); // Add to array so it gets saved
	ZoneTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication
	ZoneTimeline->SetIsReplicated(true);
	ZoneTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	ZoneTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
	ZoneTimeline->SetLooping(false);
	ZoneTimeline->RegisterComponent();

	ZoneTimeline->SetTimelineLength(ZoneTimeLineLength);

	ZoneSizeCurve = NewObject<UCurveFloat>();
	FOnTimelineFloat onZoneSizeCallback;
	onZoneSizeCallback.BindUFunction(this, FName{ TEXT("ZoneSizeUpdate") });
	ZoneTimeline->AddInterpFloat(ZoneSizeCurve, onZoneSizeCallback);
	ZoneSizeCurve->FloatCurve = ZoneSizeRichCurve;

	ZoneLocationCurve = NewObject<UCurveVector>();
	FOnTimelineVector onZoneLocationCallback;
	onZoneLocationCallback.BindUFunction(this, FName{ TEXT("ZoneLocationUpdate") });
	ZoneTimeline->AddInterpVector(ZoneLocationCurve, onZoneLocationCallback);
	ZoneLocationCurve->FloatCurves[0] = ZoneLocationRichCurveX;
	ZoneLocationCurve->FloatCurves[1] = ZoneLocationRichCurveY;
	ZoneLocationCurve->FloatCurves[2] = ZoneLocationRichCurveZ;

	if (HasAuthority())
	{
		InitialZoneLocation = GetActorLocation();
		InitialZoneSize = GetActorScale3D().X;

		FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
		LevelName.RemoveFromStart("Level_");
		MaxZoneStage = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, ZONE_STAGE_AMOUNT_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName), -1);
		if (MaxZoneStage > 0)
		{
			SIZE_WAIT_SHRINK_DAMAGE.Empty();

			for (int i = 1; i <= MaxZoneStage; i++)
			{
				FVector4 ZoneStage;
				ZoneStage.X = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					ZONE_STAGE_FINAL_SIZE_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.Y = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					ZONE_STAGE_WAIT_TIME_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.Z = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					ZONE_STAGE_SHRINK_TIME_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.W = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					ZONE_STAGE_DAMAGE_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);

				SIZE_WAIT_SHRINK_DAMAGE.Add(ZoneStage);
			}
		}
		else
		{
			MaxZoneStage = SIZE_WAIT_SHRINK_DAMAGE.Num();
		}
	}
}

void ABRZoneActor::OnRep_ZoneTimeLineLength()
{
	if (ZoneTimeline)
	{
		ZoneTimeline->SetTimelineLength(ZoneTimeLineLength);
	}
}

void ABRZoneActor::OnRep_ZoneSizeRichCurve()
{
	if (ZoneSizeCurve)
	{
		ZoneSizeCurve->FloatCurve = ZoneSizeRichCurve;
	}
}

void ABRZoneActor::OnRep_ZoneLocationRichCurveX()
{
	if (ZoneLocationCurve)
	{
		ZoneLocationCurve->FloatCurves[0] = ZoneLocationRichCurveX;
	}
}

void ABRZoneActor::OnRep_ZoneLocationRichCurveY()
{
	if (ZoneLocationCurve)
	{
		ZoneLocationCurve->FloatCurves[1] = ZoneLocationRichCurveY;
	}
}

void ABRZoneActor::OnRep_ZoneLocationRichCurveZ()
{
	if (ZoneLocationCurve)
	{
		ZoneLocationCurve->FloatCurves[2] = ZoneLocationRichCurveZ;
	}
}

void ABRZoneActor::Reset()
{
	Stop();
	CurrentZoneStage = -1;
	if (ZoneTimeline)
	{
		ZoneTimeline->SetNewTime(0);
	}
}

void ABRZoneActor::Start()
{
	if (HasAuthority())
	{
		ZoneTimeLineLength = 0;
		ZoneSizeRichCurve = FRichCurve();
		ZoneLocationRichCurveX = FRichCurve();
		ZoneLocationRichCurveY = FRichCurve();
		ZoneLocationRichCurveZ = FRichCurve();

		float ZoneStageSize = InitialZoneSize;
		FKeyHandle Key = ZoneSizeRichCurve.AddKey(0, ZoneStageSize);
		ZoneSizeRichCurve.SetKeyInterpMode(Key, RCIM_Linear);

		FVector ZoneStageLocation = InitialZoneLocation;
		Key = ZoneLocationRichCurveX.AddKey(0, ZoneStageLocation.X);
		ZoneLocationRichCurveX.SetKeyInterpMode(Key, RCIM_Linear);
		Key = ZoneLocationRichCurveY.AddKey(0, ZoneStageLocation.Y);
		ZoneLocationRichCurveY.SetKeyInterpMode(Key, RCIM_Linear);
		Key = ZoneLocationRichCurveZ.AddKey(0, ZoneStageLocation.Z);
		ZoneLocationRichCurveZ.SetKeyInterpMode(Key, RCIM_Linear);

		for (int i = 0; i < SIZE_WAIT_SHRINK_DAMAGE.Num(); i++)
		{
			ZoneTimeLineLength += SIZE_WAIT_SHRINK_DAMAGE[i].Y;
			ZoneSizeRichCurve.AddKey(ZoneTimeLineLength, ZoneStageSize);
			ZoneLocationRichCurveX.AddKey(ZoneTimeLineLength, ZoneStageLocation.X);
			ZoneLocationRichCurveY.AddKey(ZoneTimeLineLength, ZoneStageLocation.Y);
			ZoneLocationRichCurveZ.AddKey(ZoneTimeLineLength, ZoneStageLocation.Z);

			ZoneStageLocation = GetNextZoneStageLocation(ZoneStageLocation, ZoneStageSize - SIZE_WAIT_SHRINK_DAMAGE[i].X);

			ZoneTimeLineLength += SIZE_WAIT_SHRINK_DAMAGE[i].Z;
			ZoneStageSize = SIZE_WAIT_SHRINK_DAMAGE[i].X;
			ZoneSizeRichCurve.AddKey(ZoneTimeLineLength, ZoneStageSize);
			ZoneLocationRichCurveX.AddKey(ZoneTimeLineLength, ZoneStageLocation.X);
			ZoneLocationRichCurveY.AddKey(ZoneTimeLineLength, ZoneStageLocation.Y);
			ZoneLocationRichCurveZ.AddKey(ZoneTimeLineLength, ZoneStageLocation.Z);
		}

		OnRep_ZoneTimeLineLength();
		OnRep_ZoneSizeRichCurve();
		OnRep_ZoneLocationRichCurveX();
		OnRep_ZoneLocationRichCurveY();
		OnRep_ZoneLocationRichCurveZ();

		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &ABRZoneActor::DamagePlayerOutsideZone, 1.0, true);
		ZoneTimeline->Play();
		StartNextZoneStage();
	}
}

void ABRZoneActor::Stop()
{
	if (HasAuthority())
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		}
		if (ZoneTimeline)
		{
			ZoneTimeline->Stop();
		}
	}
}

int ABRZoneActor::GetTimeToFullyShrink()
{
	int FullTime = 0;

	for (int i = 0; i < SIZE_WAIT_SHRINK_DAMAGE.Num(); i++)
	{
		FullTime += SIZE_WAIT_SHRINK_DAMAGE[i].Y;
		FullTime += SIZE_WAIT_SHRINK_DAMAGE[i].Z;
	}

	return FullTime;
}

void ABRZoneActor::StartNextZoneStage()
{
	CurrentZoneStage++;

	if (CurrentZoneStage >= MaxZoneStage)
	{
		return;
	}

	if (SIZE_WAIT_SHRINK_DAMAGE[CurrentZoneStage].Y == 0)
	{
		OnZoneShrinkStarted();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ZoneWaitTimerHandle, this, &ABRZoneActor::OnZoneShrinkStarted, SIZE_WAIT_SHRINK_DAMAGE[CurrentZoneStage].Y, false);
	}
}

void ABRZoneActor::OnZoneShrinkStarted()
{
	CurrentZoneDamage = SIZE_WAIT_SHRINK_DAMAGE[CurrentZoneStage].W;

	if (SIZE_WAIT_SHRINK_DAMAGE[CurrentZoneStage].Z == 0)
	{
		StartNextZoneStage();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ZoneMovingTimerHandle, this, &ABRZoneActor::StartNextZoneStage, SIZE_WAIT_SHRINK_DAMAGE[CurrentZoneStage].Z, false);
	}
}

void ABRZoneActor::DamagePlayerOutsideZone()
{
	for (TActorIterator<APlayerCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerCharacter* PlayerCharacter = *ActorItr;

		if (CurrentZoneStage >= MaxZoneStage || FVector::DistXY(GetActorLocation(), PlayerCharacter->GetActorLocation()) > (GetActorScale3D().X * 50))
		{
			PlayerCharacter->TakeDamage(CurrentZoneDamage, FDamageEvent(UZoneDamageType::StaticClass()), nullptr, nullptr);
		}
	}
}
