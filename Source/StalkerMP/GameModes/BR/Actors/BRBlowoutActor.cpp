// Fill out your copyright notice in the Description page of Project Settings.


#include "BRBlowoutActor.h"

#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"
#include "StalkerMP/GameModes/BR/Actors/BRLevelDataActor.h"
#include "StalkerMP/GameModes/BR/Actors/BRSafeZoneActor.h"
#include "StalkerMP/DamageTypes/ZoneDamageType.h"
#include "StalkerMP/Levels/WeatherActor.h"
#include "StalkerMP/StalkerMPGameInstance.h"
#include "StalkerMP/SMPFunctions.h"

#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Public/TimerManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

const FString ABRBlowoutActor::BLOWOUT_STAGE_AMOUNT_SETTING_KEY = "BR_{Map}_BlowoutStageAmount";
const FString ABRBlowoutActor::BLOWOUT_STAGE_WAIT_TIME_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_WaitTime";
const FString ABRBlowoutActor::BLOWOUT_STAGE_WARNING_TIME_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_WarningTime";
const FString ABRBlowoutActor::BLOWOUT_STAGE_DURATION_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_Duration";
const FString ABRBlowoutActor::BLOWOUT_STAGE_DAMAGE_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_Damage";
const FString ABRBlowoutActor::BLOWOUT_STAGE_SAFECOUNT_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_SafeCount";
const FString ABRBlowoutActor::BLOWOUT_STAGE_SAFESIZESTART_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_SafeSizeStart";
const FString ABRBlowoutActor::BLOWOUT_STAGE_SAFESIZEEND_SETTING_KEY = "BR_{Map}_BlowoutStage{StageNum}_SafeSizeEnd";

ABRBlowoutActor::ABRBlowoutActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	SetReplicates(true);
	NetCullDistanceSquared = 40000000000.0;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetMobility(EComponentMobility::Static);
	SpawnBox->SetBoxExtent(FVector(55000, 55000, 5000), false);
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
}

void ABRBlowoutActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABRBlowoutActor, IsEnabled);
	DOREPLIFETIME(ABRBlowoutActor, OutsideSafetyLevel);
}

void ABRBlowoutActor::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AWeatherActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		WeatherActor = *ActorItr;
		break;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABRLevelDataActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ABRLevelDataActor* LevelDataActor = (ABRLevelDataActor*)FoundActors[0];

		CmPerPixel = LevelDataActor->MapCmPerPixel;
		TextureSize = LevelDataActor->MapTextureSize;
		TextureOffset = LevelDataActor->MapTextureOffset;
	}

	if (HasAuthority())
	{
		BlowoutPrepareScale = BlowoutPrepareScale / (1.0 - BlowoutPrepareScale * 2);

		FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
		LevelName.RemoveFromStart("Level_");
		MaxBlowoutStage = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server, BLOWOUT_STAGE_AMOUNT_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName), -1);
		if (MaxBlowoutStage > 0)
		{
			WAIT_WARNING_DURATION_DAMAGE.Empty();
			SAFECOUNT_SIZESTART_SIZEEND.Empty();

			for (int i = 1; i <= MaxBlowoutStage; i++)
			{
				FVector4 ZoneStage;
				ZoneStage.X = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_WAIT_TIME_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.Y = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_WARNING_TIME_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.Z = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_DURATION_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.W = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_DAMAGE_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);

				WAIT_WARNING_DURATION_DAMAGE.Add(ZoneStage);
				SecondsTillTheEnd += ZoneStage.X + ZoneStage.Y + ZoneStage.Z;

				FVector SafeZoneStage;
				ZoneStage.X = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_SAFECOUNT_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.Y = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_SAFESIZESTART_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);
				ZoneStage.Z = GetGameInstance<UStalkerMPGameInstance>()->GetIntSettingValue(ESettingsType::Server,
					BLOWOUT_STAGE_SAFESIZEEND_SETTING_KEY.Replace(TEXT("{Map}"), *LevelName).Replace(TEXT("{StageNum}"), *FString::FromInt(i)), 1);

				SAFECOUNT_SIZESTART_SIZEEND.Add(SafeZoneStage);
			}
		}
		else
		{
			MaxBlowoutStage = WAIT_WARNING_DURATION_DAMAGE.Num();

			for (int i = 0; i < WAIT_WARNING_DURATION_DAMAGE.Num(); i++)
			{
				SecondsTillTheEnd += WAIT_WARNING_DURATION_DAMAGE[i].X + WAIT_WARNING_DURATION_DAMAGE[i].Y + WAIT_WARNING_DURATION_DAMAGE[i].Z;
			}
		}
	}
}

void ABRBlowoutActor::SetEnabled(bool Enabled)
{
	IsEnabled = Enabled;
}

void ABRBlowoutActor::OnRep_IsEnabled()
{
	if (IsEnabled)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetHidden(true);
		MarkComponentsRenderStateDirty();
	}
}

void ABRBlowoutActor::Reset()
{
	Stop();
	CurrentBlowoutStage = -1;
}

void ABRBlowoutActor::Start(FTimecode MatchStartTime, FTimecode WeatherTimePass)
{
	TArray<FVector4> TimeScales;
	float Time = 0;
	for (int i = 0; i < WAIT_WARNING_DURATION_DAMAGE.Num(); i++)
	{
		FVector4 ZoneStage = WAIT_WARNING_DURATION_DAMAGE[i];
		float BlowoutPrepareOffset = ZoneStage.Z * BlowoutPrepareScale;

		FVector4 TimeScale;

		if (i > 0)
		{
			Time -= WAIT_WARNING_DURATION_DAMAGE[i - 1].Y;
		}
		Time += ZoneStage.X;
		TimeScale.X = Time;
		Time += (ZoneStage.Y - BlowoutPrepareOffset);
		TimeScale.Y = Time;
		Time += (ZoneStage.Z + BlowoutPrepareOffset * 2);
		TimeScale.Z = Time;
		Time += (ZoneStage.Y - BlowoutPrepareOffset);
		TimeScale.W = Time;

		TimeScales.Add(TimeScale);
	}

	float TimepassScale = USMPFunctions::TimecodeToSeconds(WeatherTimePass) / SecondsTillTheEnd;
	float MatchStartSeconds = USMPFunctions::TimecodeToSeconds(MatchStartTime);
	for (int i = 0; i < TimeScales.Num(); i++)
	{
		FVector4 TimeScale = TimeScales[i];

		FTimecode StartTime = USMPFunctions::SecondsToTimecode(MatchStartSeconds + TimeScale.X * TimepassScale);
		FTimecode StartFullTime = USMPFunctions::SecondsToTimecode(MatchStartSeconds + TimeScale.Y * TimepassScale);
		FTimecode EndFullTime = USMPFunctions::SecondsToTimecode(MatchStartSeconds + TimeScale.Z * TimepassScale);
		FTimecode EndTime = USMPFunctions::SecondsToTimecode(MatchStartSeconds + TimeScale.W * TimepassScale);

		if (StartFullTime.Hours >= 6 && StartFullTime.Hours < 18)
		{
			WeatherActor->OverrideWeatherSpecial(BlowoutDayWeatherName, StartTime, StartFullTime, EndFullTime, EndTime);
		}
		else
		{
			WeatherActor->OverrideWeatherSpecial(BlowoutNightWeatherName, StartTime, StartFullTime, EndFullTime, EndTime);
		}
	}

	StartNextBlowoutStage();
}

void ABRBlowoutActor::Stop()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}

void ABRBlowoutActor::StartNextBlowoutStage()
{
	CurrentBlowoutStage++;

	if (CurrentBlowoutStage >= MaxBlowoutStage)
	{
		return;
	}

	OutsideSafetyLevel = 1;
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);

	int NextBlowoutDelay = WAIT_WARNING_DURATION_DAMAGE[CurrentBlowoutStage].X;
	GetWorld()->GetTimerManager().SetTimer(BlowoutTimerHandle, this, &ABRBlowoutActor::WarnAboutBlowout, NextBlowoutDelay, false);
}

void ABRBlowoutActor::WarnAboutBlowout()
{
	OutsideSafetyLevel = 0.5;

	FVector SafeZone = SAFECOUNT_SIZESTART_SIZEEND[CurrentBlowoutStage];
	TArray<FVector> SpawnedZones;
	for (int i = 0; i < SafeZone.X; i++)
	{
		FTransform SpawnTransform = FTransform();

		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), SpawnBox->GetScaledBoxExtent());
		while (NewSafeZoneOverlapsOld(SpawnLocation, SafeZone.Y, SpawnedZones))
		{
			SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), SpawnBox->GetScaledBoxExtent());
		}
		SpawnLocation.Z = GetActorLocation().Z + SpawnBox->GetScaledBoxExtent().Z;
		LinetraceZ(SpawnLocation, GetActorLocation().Z - SpawnBox->GetScaledBoxExtent().Z);

		SpawnTransform.SetLocation(SpawnLocation);

		FActorSpawnParameters SpawnInfo = FActorSpawnParameters();
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABRSafeZoneActor* SafeZoneActor = GetWorld()->SpawnActor<ABRSafeZoneActor>(SafeZoneActorClass, SpawnTransform, SpawnInfo);
		SafeZoneActor->SetParams(SafeZone.Y, SafeZone.Z, WAIT_WARNING_DURATION_DAMAGE[CurrentBlowoutStage].Y, WAIT_WARNING_DURATION_DAMAGE[CurrentBlowoutStage].Z, true);

		SpawnedZones.Add(FVector(SpawnLocation.X, SpawnLocation.Y, SafeZone.Y));
	}

	int NextBlowoutDelay = WAIT_WARNING_DURATION_DAMAGE[CurrentBlowoutStage].Y;
	GetWorld()->GetTimerManager().SetTimer(BlowoutTimerHandle, this, &ABRBlowoutActor::StartDealingDamage, NextBlowoutDelay, false);
}

void ABRBlowoutActor::StartDealingDamage()
{
	OutsideSafetyLevel = 0;

	CurrentBlowoutDamage = WAIT_WARNING_DURATION_DAMAGE[CurrentBlowoutStage].W;
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &ABRBlowoutActor::DamagePlayerOutside, 1.0, true);

	int NextBlowoutDelay = WAIT_WARNING_DURATION_DAMAGE[CurrentBlowoutStage].Z;
	GetWorld()->GetTimerManager().SetTimer(BlowoutTimerHandle, this, &ABRBlowoutActor::StartNextBlowoutStage, NextBlowoutDelay, false);
}

bool ABRBlowoutActor::NewSafeZoneOverlapsOld(FVector NewLocation, float Size, TArray<FVector> &SpawnedZones)
{
	for (FVector SpawnedZone : SpawnedZones)
	{
		if (FVector::Dist2D(NewLocation, FVector(SpawnedZone.X, SpawnedZone.Y, 0)) < (Size + SpawnedZone.Z) * 100)
		{
			return true;
		}
	}

	return false;
}

void ABRBlowoutActor::LinetraceZ(FVector &SpawnLocation, float BottomZ)
{
	FVector Start = SpawnLocation;
	FVector End = FVector(SpawnLocation.X, SpawnLocation.Y, BottomZ);

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	FCollisionResponseParams CollisionResponseParams = FCollisionResponseParams();

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionQueryParams, CollisionResponseParams);

	SpawnLocation.Z = HitResult.Location.Z;
}

void ABRBlowoutActor::DamagePlayerOutside()
{
	for (TActorIterator<APlayerCharacter> PlayerItr(GetWorld()); PlayerItr; ++PlayerItr)
	{
		APlayerCharacter* PlayerCharacter = *PlayerItr;

		bool InsideSafeZone = false;
		if (CurrentBlowoutStage < MaxBlowoutStage)
		{
			for (TActorIterator<ABRSafeZoneActor> SafeZoneItr(GetWorld()); SafeZoneItr; ++SafeZoneItr)
			{
				if (FVector::Dist((*SafeZoneItr)->GetActorLocation(), PlayerCharacter->GetActorLocation()) < ((*SafeZoneItr)->GetActorScale3D().X * 50))
				{
					InsideSafeZone = true;
					break;
				}
			}
		}

		if (!InsideSafeZone)
		{
			PlayerCharacter->TakeDamage(CurrentBlowoutDamage, FDamageEvent(UZoneDamageType::StaticClass()), nullptr, nullptr);
		}
	}
}

int ABRBlowoutActor::GetTimeTillTheEnd()
{
	return SecondsTillTheEnd;
}

void ABRBlowoutActor::GetSafeZonesCoords(TArray<FVector> &SafeZonesCoords)
{
	for (TActorIterator<ABRSafeZoneActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		SafeZonesCoords.Add(FVector((*ActorItr)->GetActorLocation().X / TextureSize.X / CmPerPixel + 0.5,
			(*ActorItr)->GetActorLocation().Y / TextureSize.Y / CmPerPixel + 0.5,
			(*ActorItr)->GetActorScale3D().X * 100 / TextureSize.X / CmPerPixel));
	}
}

float ABRBlowoutActor::GetOutsideSafetyLevel()
{
	return OutsideSafetyLevel;
}
