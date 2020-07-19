// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherActor.h"

#include "StalkerMP/Levels/WorldAmbientEffectManager.h"
#include "StalkerMP/SMPFunctions.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Particles/ParticleSystemComponent.h"
#include "Curves/CurveFloat.h"
#include "Camera/PlayerCameraManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"



AWeatherActor::AWeatherActor()
{
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	NetCullDistanceSquared = 40000000000.0;

	DefaultComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultComponent"));
	DefaultComponent->SetMobility(EComponentMobility::Static);
	RootComponent = DefaultComponent;

	SkyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkyMesh"));
	SkyMesh->SetMobility(EComponentMobility::Static);
	SkyMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SkyMesh->SetCastShadow(false);
	SkyMesh->SetupAttachment(RootComponent);

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLight->SetMobility(EComponentMobility::Movable);
	SkyLight->SourceType = ESkyLightSourceType::SLS_CapturedScene;
	SkyLight->CastShadows = false;
	SkyLight->SetupAttachment(RootComponent);

	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	SunLight->SetMobility(EComponentMobility::Movable);
	SunLight->bEnableLightShaftBloom = true;
	SunLight->Intensity = 2.0;
	SunLight->BloomScale = 0.025;
	SunLight->BloomMaxBrightness = 10;
	SunLight->bUseRayTracedDistanceFieldShadows = true;
	SunLight->DistanceFieldShadowDistance = 200000;
	SunLight->SetupAttachment(RootComponent);

	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
	ExponentialHeightFog->SetMobility(EComponentMobility::Static);
	ExponentialHeightFog->SetAbsolute(false, false, true);
	ExponentialHeightFog->SetupAttachment(RootComponent);

	RainParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RainParticleSystem"));
	RainParticleSystem->SetMobility(EComponentMobility::Movable);
	RainParticleSystem->SetAbsolute(false, false, true);
	RainParticleSystem->SetupAttachment(RootComponent);

	RainDepthCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("RainDepthCaptureComponent2D"));
	RainDepthCaptureComponent2D->SetMobility(EComponentMobility::Movable);
	RainDepthCaptureComponent2D->SetAbsolute(false, false, true);
	RainDepthCaptureComponent2D->SetRelativeRotation(FRotator(-90, 0, 0).Quaternion());
	RainDepthCaptureComponent2D->SetRelativeLocation(FVector(0, 0, 2560));
	RainDepthCaptureComponent2D->ProjectionType = ECameraProjectionMode::Orthographic;
	RainDepthCaptureComponent2D->OrthoWidth = 4096;
	RainDepthCaptureComponent2D->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	RainDepthCaptureComponent2D->CaptureSource = ESceneCaptureSource::SCS_SceneDepth;
	RainDepthCaptureComponent2D->bCaptureEveryFrame = true;
	RainDepthCaptureComponent2D->bCaptureOnMovement = false;
	RainDepthCaptureComponent2D->MaxViewDistanceOverride = 2560;
	RainDepthCaptureComponent2D->PostProcessSettings.BloomIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.LensFlareIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.VignetteIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.AmbientCubemapIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.AmbientOcclusionIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.RayTracingAO = false;
	RainDepthCaptureComponent2D->PostProcessSettings.IndirectLightingIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.MotionBlurAmount = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.LPVIntensity = 0;
	RainDepthCaptureComponent2D->PostProcessSettings.ReflectionsType = EReflectionsType::ScreenSpace;
	RainDepthCaptureComponent2D->PostProcessSettings.ScreenSpaceReflectionIntensity = 0;
	RainDepthCaptureComponent2D->ShowFlags.AntiAliasing = false;
	RainDepthCaptureComponent2D->ShowFlags.Atmosphere = false;
	RainDepthCaptureComponent2D->ShowFlags.BSP = true;
	RainDepthCaptureComponent2D->ShowFlags.Decals = false;
	RainDepthCaptureComponent2D->ShowFlags.Fog = false;
	RainDepthCaptureComponent2D->ShowFlags.Landscape = true;
	RainDepthCaptureComponent2D->ShowFlags.Particles = false;
	RainDepthCaptureComponent2D->ShowFlags.SkeletalMeshes = true;
	RainDepthCaptureComponent2D->ShowFlags.StaticMeshes = true;
	RainDepthCaptureComponent2D->ShowFlags.Translucency = true;
	RainDepthCaptureComponent2D->ShowFlags.DeferredLighting = false;
	RainDepthCaptureComponent2D->ShowFlags.InstancedFoliage = false;
	RainDepthCaptureComponent2D->ShowFlags.InstancedGrass = false;
	RainDepthCaptureComponent2D->ShowFlags.InstancedStaticMeshes = true;
	RainDepthCaptureComponent2D->ShowFlags.Paper2DSprites = false;
	RainDepthCaptureComponent2D->ShowFlags.TextRender = false;
	RainDepthCaptureComponent2D->ShowFlags.TemporalAA = false;
	RainDepthCaptureComponent2D->ShowFlags.Bloom = false;
	RainDepthCaptureComponent2D->ShowFlags.EyeAdaptation = false;
	RainDepthCaptureComponent2D->ShowFlags.MotionBlur = false;
	RainDepthCaptureComponent2D->ShowFlags.SkyLighting = false;
	RainDepthCaptureComponent2D->ShowFlags.AmbientOcclusion = false;
	RainDepthCaptureComponent2D->ShowFlags.DynamicShadows = false;
	RainDepthCaptureComponent2D->ShowFlags.AmbientCubemap = false;
	RainDepthCaptureComponent2D->ShowFlags.DistanceFieldAO = false;
	RainDepthCaptureComponent2D->ShowFlags.LightFunctions = false;
	RainDepthCaptureComponent2D->ShowFlags.LightShafts = false;
	RainDepthCaptureComponent2D->ShowFlags.ReflectionEnvironment = false;
	RainDepthCaptureComponent2D->ShowFlags.ScreenSpaceReflections = false;
	RainDepthCaptureComponent2D->ShowFlags.TexturedLightProfiles = false;
	RainDepthCaptureComponent2D->ShowFlags.VolumetricFog = false;
	RainDepthCaptureComponent2D->ShowFlags.Game = false;
	RainDepthCaptureComponent2D->ShowFlags.Lighting = false;
	RainDepthCaptureComponent2D->ShowFlags.PostProcessing = false;
	RainDepthCaptureComponent2D->SetupAttachment(RainParticleSystem);
}

void AWeatherActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeatherActor, CurrentWeatherDatas, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, CurrentWeatherTypes, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, ChangingTimeRichCurve, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, TimelineLength, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, TimelinePosition, COND_InitialOnly);
}

void AWeatherActor::BeginPlay()
{
	Super::BeginPlay();

	ChangingWeatherTimeline = NewObject<UTimelineComponent>(this, FName("WeatherTimeline"));
	ChangingWeatherTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	//this->BlueprintCreatedComponents.Add(ChangingWeatherTimeline); // Add to array so it gets saved
	//ChangingWeatherTimeline->SetNetAddressable(); // This component has a stable name that can be referenced for replication
	ChangingWeatherTimeline->SetIsReplicated(true);
	ChangingWeatherTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	ChangingWeatherTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
	ChangingWeatherTimeline->SetLooping(false);
	ChangingWeatherTimeline->RegisterComponent();

	ChangingTimeCurve = NewObject<UCurveFloat>();
	FOnTimelineFloat onTimeChangeCallback;
	onTimeChangeCallback.BindUFunction(this, FName{ TEXT("ApplyCurrentWeatherTimeline") });
	ChangingWeatherTimeline->AddInterpFloat(ChangingTimeCurve, onTimeChangeCallback);

	TArray<FString> WeatherTypes;
	Weathers.GetKeys(WeatherTypes);
	AllowedWeathers = WeatherTypes;
	CurrentWeatherType = WeatherTypes[0];
	NextWeatherType = WeatherTypes[0];

	if (HasAuthority())
	{
		RainParticleSystem->Deactivate();
		RainDepthCaptureComponent2D->Deactivate();

		CurrentWeatherDatas = Weathers[CurrentWeatherType].Hours;
		for (int i = 0; i < CurrentWeatherDatas.Num(); i++)
		{
			CurrentWeatherTypes.Add(CurrentWeatherType);
		}

		int Hour = UKismetMathLibrary::RandomInteger(24);
		int Minute = UKismetMathLibrary::RandomInteger(60);
		FTimecode Time = FTimecode(Hour, Minute, 0, 0, false);
		CurrentWeather = FindWeatherData(Time, CurrentWeatherType);
		NextWeather = FindNextWeatherData(Time, CurrentWeatherType);
		SetTimeOfDay(Time, 0, false);
	}
	else
	{
		OnEverythingReplicated();
	}

	AWorldAmbientEffectManager* WorldAmbientEffectManager = GetWorld()->SpawnActor<AWorldAmbientEffectManager>(AWorldAmbientEffectManager::StaticClass(), FActorSpawnParameters());
	WorldAmbientEffectManager->SetWeatherActor(this);
}

void AWeatherActor::Tick(float DeltaSeconds)
{
	if (AttachedCamera)
	{
		RainParticleSystem->SetWorldLocation(AttachedCamera->GetCameraLocation() + AttachedCamera->GetVelocity());
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MaterialParameterCollection,
			RainEmitterLocationParameterName, RainDepthCaptureComponent2D->GetComponentLocation());
	}
}

void AWeatherActor::OnRep_TimelinePosition()
{
	TimelinePositionReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_TimelineLength()
{
	TimelineLengthReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_ChangingTimeRichCurve()
{
	ChangingTimeRichCurveReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_CurrentWeatherDatas()
{
	CurrentWeatherDatasReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_CurrentWeatherTypes()
{
	CurrentWeatherTypesReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnEverythingReplicated()
{
	if (TimelinePositionReplicated && TimelineLengthReplicated && ChangingTimeRichCurveReplicated && CurrentWeatherDatasReplicated && CurrentWeatherTypesReplicated
		&& ChangingWeatherTimeline)
	{
		CurrentWeather = CurrentWeatherDatas[0];
		CurrentWeatherType = CurrentWeatherTypes[0];
		NextWeather = CurrentWeatherDatas[1];
		NextWeatherType = CurrentWeatherTypes[1];
		WeatherDataIsReady = true;

		ChangingTimeCurve->FloatCurve = ChangingTimeRichCurve;
		ChangingWeatherTimeline->SetTimelineLength(TimelineLength);
		ChangingWeatherTimeline->SetPlaybackPosition(TimelinePosition, false, true);
		ChangingWeatherTimeline->Play();
	}
}

void AWeatherActor::ApplyCurrentWeatherTimeline(float Val)
{
	if (!WeatherDataIsReady || CurrentWeatherDatas.Num() < 2)
	{
		return;
	}
	TimelinePosition = ChangingWeatherTimeline->GetPlaybackPosition();

	while (CurrentWeatherDatas.Num() >= 2
		&& (
			(USMPFunctions::TimecodeToSeconds(NextWeather.StartTime) <= Val
				&& USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) < USMPFunctions::TimecodeToSeconds(NextWeather.StartTime))
			||
			(USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) > USMPFunctions::TimecodeToSeconds(NextWeather.StartTime)
				&& Val < USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime)
				&& Val >= USMPFunctions::TimecodeToSeconds(NextWeather.StartTime))
			||
			(Val < USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime)
			&& Val < USMPFunctions::TimecodeToSeconds(NextWeather.StartTime)))
		)
	{
		if (CurrentWeatherDatas.Num() > 2)
		{
			CurrentWeatherDatas.RemoveAt(0);
			CurrentWeatherTypes.RemoveAt(0);
		}

		CurrentWeather = CurrentWeatherDatas[0];
		CurrentWeatherType = CurrentWeatherTypes[0];
		NextWeather = CurrentWeatherDatas[1];
		NextWeatherType = CurrentWeatherTypes[1];

		if (!HasAuthority())
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, CurrentWeather.StartTime.ToString() + " - " + CurrentWeatherType);

		CurrentTime = Val;

		if (CurrentWeatherDatas.Num() <= 2)
		{
			break;
		}
	}

	CurrentTime = Val;

	if (!HasAuthority())
	{
		Update(Val, CurrentWeather, NextWeather);
	}
}

void AWeatherActor::Update(float Time, FWeatherTimeOfDayData WeatherData, FWeatherTimeOfDayData NextWeatherData)
{
	FWeatherTimeOfDayData LerpedWeather = LerpWeatherF(WeatherData, NextWeatherData, Time);
	WeatherLerpValue = CalculateWeatherLerpF(WeatherData.StartTime, NextWeatherData.StartTime, Time);

	if (!HasAuthority())
		if (WeatherLerpValue < 0 || WeatherLerpValue > 1)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(Time) + " - " + WeatherData.StartTime.ToString() + " - " + NextWeatherData.StartTime.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(WeatherLerpValue));
		}

	FRotator SunRotation = FRotator(LerpedWeather.SunLongitude, 0, 0);
	FRotator SunAltitudeRotation = FRotator(0, LerpedWeather.SunAltitude, 0);

	FQuat CurrentSunRotation = SunAltitudeRotation.Quaternion() * SunRotation.Quaternion();

	if (!SkyMaterialDynamic)
	{
		SkyMaterialDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), SkyMaterial);
		SkyMesh->SetMaterial(0, SkyMaterialDynamic);
	}
	if (WeatherData.SkyCubemap)
	{
		SkyMaterialDynamic->SetTextureParameterValue(CurrentSkyTextureParameterName, (UTexture*)WeatherData.SkyCubemap);
	}
	if (NextWeatherData.SkyCubemap)
	{
		SkyMaterialDynamic->SetTextureParameterValue(NextSkyTextureParameterName, (UTexture*)NextWeatherData.SkyCubemap);
		SkyMaterialDynamic->SetScalarParameterValue(LerpParameterName, WeatherLerpValue);
	}
	SkyMaterialDynamic->SetVectorParameterValue(SunLightColorParameterName, LerpedWeather.SunLightColor);
	SkyMaterialDynamic->SetVectorParameterValue(SkyColorParameterName, LerpedWeather.SkyColor);

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, WetnessParameterName, LerpedWeather.Wetness);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, WindIntensityParameterName, LerpedWeather.WindIntensity);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MaterialParameterCollection, SunLightVectorParameterName, CurrentSunRotation.Vector());
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MaterialParameterCollection, SunLightColorParameterName, LerpedWeather.SunLightColor);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, SunLightIntensityParameterName, LerpedWeather.SunLightIntensity);

	SunLight->SetWorldRotation(CurrentSunRotation);
	SunLight->SetIntensity(LerpedWeather.SunLightIntensity);
	SunLight->SetLightColor(LerpedWeather.SunLightColor);

	SkyLight->SetIntensity(LerpedWeather.SkyLightIntensity);
	SkyLight->SetLightColor(LerpedWeather.SkyLightColor);

	ExponentialHeightFog->SetFogDensity(LerpedWeather.FogDensity);
	ExponentialHeightFog->SetFogHeightFalloff(LerpedWeather.FogHeightFalloff);
	ExponentialHeightFog->SetFogInscatteringColor(LerpedWeather.FogColor);

	RainParticleSystem->SetColorParameter(RainColorParameterName, LerpedWeather.RainColor);
	RainParticleSystem->SetFloatParameter(RainIntensityParameterName, LerpedWeather.RainIntensity);
	RainParticleSystem->SetVectorParameter(RainVelocityParameterName, LerpedWeather.RainVelocity);

	if (LerpedWeather.RainIntensity > 0 || LerpedWeather.Wetness > 0)
	{
		RainDepthCaptureComponent2D->Activate();
	}
	else
	{
		RainDepthCaptureComponent2D->Deactivate();
	}
}

void AWeatherActor::AddTime(FTimecode Time, int SecondsForChange)
{
	int FinalSeconds = CurrentTime + USMPFunctions::TimecodeToSeconds(Time);
	bool ForceNextDay = false;
	if (FinalSeconds >= USMPFunctions::SECONDS_IN_DAY)
	{
		FinalSeconds -= USMPFunctions::SECONDS_IN_DAY;
		ForceNextDay = true;
	}

	SetTimeOfDay(USMPFunctions::SecondsToTimecode(FinalSeconds), SecondsForChange, ForceNextDay);
}

void AWeatherActor::SetTimeOfDay(FTimecode FinalTime, int SecondsForChange, bool ForceNextDay)
{
	Multicast_GenerateWeather(CurrentTime, FMath::Rand(), FinalTime, SecondsForChange, ForceNextDay, AllowedWeathers);
}

void AWeatherActor::Multicast_GenerateWeather_Implementation(int StartTime, int Seed, FTimecode FinalTime, int SecondsForChange, bool ForceNextDay, const TArray<FString> &TheAllowedWeathers)
{
	ValidateAndFixTimecode(FinalTime);

	CurrentTime = StartTime;
	WeatherDataIsReady = false;

	ChangingWeatherTimeline->Stop();
	int FinalTimeInSeconds = USMPFunctions::TimecodeToSeconds(FinalTime);

	FRandomStream RandomStream = FRandomStream(Seed);

	float InitialPosition;
	bool WillBeNextDay = ForceNextDay || CurrentTime >= FinalTimeInSeconds;

	ChangingTimeRichCurve = FRichCurve();
	if (SecondsForChange <= 0)
	{
		CurrentWeatherDatas.Empty();
		CurrentWeatherTypes.Empty();

		InitialPosition = 0;
		TimelineLength = 1; // Settings to 1 so that it replicates properly

		ChangingTimeRichCurve.AddKey(0, FinalTimeInSeconds);

		NextWeatherType = TheAllowedWeathers[UKismetMathLibrary::RandomInteger(TheAllowedWeathers.Num())];
		CurrentWeatherDatas.Add(FindWeatherData(FinalTime, NextWeatherType));
		CurrentWeatherTypes.Add(NextWeatherType);
		NextWeatherType = TheAllowedWeathers[UKismetMathLibrary::RandomInteger(TheAllowedWeathers.Num())];
		CurrentWeatherDatas.Add(FindNextWeatherData(FinalTime, NextWeatherType));
		CurrentWeatherTypes.Add(NextWeatherType);
	}
	else
	{
		InitialPosition = 1.0f * (CurrentTime - USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime)) / USMPFunctions::SECONDS_IN_DAY;
		TimelineLength = InitialPosition + SecondsForChange;

		if (InitialPosition > 0)
		{
			ChangingTimeRichCurve.AddKey(0, USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime));
		}
		ChangingTimeRichCurve.AddKey(InitialPosition, CurrentTime);

		if (WillBeNextDay)
		{
			float SecondsTillNextDay = USMPFunctions::SECONDS_IN_DAY - CurrentTime;
			SecondsTillNextDay = (SecondsTillNextDay / (SecondsTillNextDay + FinalTimeInSeconds)) * SecondsForChange;

			FKeyHandle ZeroKey = ChangingTimeRichCurve.AddKey(SecondsTillNextDay - 0.01f, USMPFunctions::SECONDS_IN_DAY);
			if (FinalTimeInSeconds != 0)
			{
				ChangingTimeRichCurve.SetKeyInterpMode(ZeroKey, ERichCurveInterpMode::RCIM_Constant);
				ChangingTimeRichCurve.AddKey(InitialPosition + SecondsTillNextDay, 0);
			}
		}

		ChangingTimeRichCurve.AddKey(TimelineLength, FinalTimeInSeconds);

		int IndexToDeleteFrom = -1;
		for (int i = 1; i < CurrentWeatherTypes.Num(); i++)
		{
			if (AllowedWeathers.Contains(CurrentWeatherTypes[i]))
			{
				IndexToDeleteFrom = i + 1;
				break;
			}
		}

		if (IndexToDeleteFrom == -1)
		{
			CurrentWeather = CurrentWeatherDatas[CurrentWeatherDatas.Num() - 1];
			CurrentWeatherType = CurrentWeatherTypes[CurrentWeatherTypes.Num() - 1];
			NextWeatherType = TheAllowedWeathers[UKismetMathLibrary::RandomInteger(TheAllowedWeathers.Num())];
			NextWeather = FindNextWeatherData(CurrentWeather.StartTime, NextWeatherType);
			CurrentWeatherDatas.Add(NextWeather);
			CurrentWeatherTypes.Add(NextWeatherType);
		}
		else
		{
			while (CurrentWeatherDatas.Num() > IndexToDeleteFrom)
			{
				CurrentWeatherDatas.RemoveAt(IndexToDeleteFrom);
				CurrentWeatherTypes.RemoveAt(IndexToDeleteFrom);
			}
			NextWeather = CurrentWeatherDatas[CurrentWeatherDatas.Num() - 1];
			NextWeatherType = CurrentWeatherTypes[CurrentWeatherTypes.Num() - 1];
			CurrentWeather = NextWeather;
			CurrentWeatherType = NextWeatherType;
		}

		while (WillBeNextDay || USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) <= FinalTimeInSeconds)
		{
			NextWeatherType = CalculateNextWeatherType(RandomStream, TheAllowedWeathers);

			NextWeather = FindNextWeatherData(CurrentWeather.StartTime, NextWeatherType);
			CurrentWeatherDatas.Add(NextWeather);
			CurrentWeatherTypes.Add(NextWeatherType);

			if (USMPFunctions::TimecodeToSeconds(NextWeather.StartTime) <= USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime))
			{
				WillBeNextDay = false;

				if (USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) < FinalTimeInSeconds)
				{
					break;
				}
			}

			CurrentWeather = NextWeather;
		}
	}

	CurrentWeather = CurrentWeatherDatas[0];
	CurrentWeatherType = CurrentWeatherTypes[0];
	NextWeather = CurrentWeatherDatas[1];
	NextWeatherType = CurrentWeatherTypes[1];

	ChangingTimeCurve->FloatCurve = ChangingTimeRichCurve;
	ChangingWeatherTimeline->SetTimelineLength(TimelineLength);

	ValidateAndFixCurrentWeatherDatas();

	WeatherDataIsReady = true;
	ChangingWeatherTimeline->SetPlaybackPosition(InitialPosition, true, true);
	ChangingWeatherTimeline->Play();
}

void AWeatherActor::OverrideWeatherNormal(const TArray<FString> &BetweenWeatherTypes, FTimecode StartTime, const TArray<FTimecode> &BetweenTimes, FTimecode EndTime, bool KeepSunMovement)
{
	Multicast_OverrideWeatherNormal(BetweenWeatherTypes, StartTime, BetweenTimes, EndTime, KeepSunMovement);
}

void AWeatherActor::Multicast_OverrideWeatherNormal_Implementation(const TArray<FString> &BetweenWeatherTypesConst, FTimecode StartTime, const TArray<FTimecode> &BetweenTimesConst, FTimecode EndTime, bool KeepSunMovement)
{
	TArray<FString> BetweenWeatherTypes = BetweenWeatherTypesConst;
	TArray<FWeatherTimeOfDayData> BetweenWeathers;
	for (int i = 0; i < BetweenTimesConst.Num(); i++)
	{
		FWeatherTimeOfDayData WeatherData = FindWeatherData(BetweenTimesConst[i], BetweenWeatherTypes[i]);
		WeatherData.StartTime = BetweenTimesConst[i];
		BetweenWeathers.Add(WeatherData);
	}

	DoOverrideWeather(BetweenWeathers, BetweenWeatherTypes, StartTime, EndTime, KeepSunMovement);
}

void AWeatherActor::OverrideWeatherSpecial(FString WeatherType, FTimecode StartTime, FTimecode StartFullTime, FTimecode EndFullTime, FTimecode EndTime)
{
	Multicast_OverrideWeatherSpecial(WeatherType, StartTime, StartFullTime, EndFullTime, EndTime);
}

void AWeatherActor::Multicast_OverrideWeatherSpecial_Implementation(const FString &WeatherType, FTimecode StartTime, FTimecode StartFullTime, FTimecode EndFullTime, FTimecode EndTime)
{
	int StartFullTimeSeconds = USMPFunctions::TimecodeToSeconds(StartFullTime);
	int EndFullTimeSeconds = USMPFunctions::TimecodeToSeconds(EndFullTime);

	TArray<FWeatherTimeOfDayData> WeathersToInsert = SpecialWeathers[WeatherType].Hours;

	int WeathersToInsertTimespan = USMPFunctions::TimecodeToSeconds(WeathersToInsert[WeathersToInsert.Num() - 1].StartTime);
	int OverrideTimeDiff = EndFullTimeSeconds - StartFullTimeSeconds;
	if (OverrideTimeDiff < 0)
	{
		OverrideTimeDiff += USMPFunctions::SECONDS_IN_DAY;
	}
	float TimeScale = 1.0f * OverrideTimeDiff / WeathersToInsertTimespan;

	for (int i = 0; i < WeathersToInsert.Num(); i++)
	{
		int WeatherSeconds = USMPFunctions::TimecodeToSeconds(WeathersToInsert[i].StartTime);
		WeatherSeconds *= TimeScale;
		WeatherSeconds += StartFullTimeSeconds;
		if (WeatherSeconds >= USMPFunctions::SECONDS_IN_DAY)
		{
			WeatherSeconds -= USMPFunctions::SECONDS_IN_DAY;
		}
		WeathersToInsert[i].StartTime = USMPFunctions::SecondsToTimecode(WeatherSeconds);
	}

	DoOverrideWeather(WeathersToInsert, SpecialWeathers[WeatherType].WeatherTypes, StartTime, EndTime, false);
}

void AWeatherActor::DoOverrideWeather(TArray<FWeatherTimeOfDayData> &WeathersToInsert, TArray<FString> &WeatherTypesToInsert, FTimecode StartTime, FTimecode EndTime, bool KeepSunMovement)
{
	ValidateAndFixTimecode(StartTime);
	ValidateAndFixTimecode(EndTime);

	int StartTimeValue = USMPFunctions::TimecodeToSeconds(StartTime);
	int EndTimeValue = USMPFunctions::TimecodeToSeconds(EndTime);

	if (StartTimeValue < USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[0].StartTime))
	{
		StartTimeValue += USMPFunctions::SECONDS_IN_DAY;
	}
	TArray<int> BetweenTimeValues;
	TArray<int> BetweenIndexes;
	int LastTimeValue = StartTimeValue;
	for (FWeatherTimeOfDayData& BetweenWeather : WeathersToInsert)
	{
		ValidateAndFixTimecode(BetweenWeather.StartTime);
		int TimeValue = USMPFunctions::TimecodeToSeconds(BetweenWeather.StartTime);
		if (TimeValue < LastTimeValue)
		{
			TimeValue += USMPFunctions::SECONDS_IN_DAY;
		}
		LastTimeValue = TimeValue;
		BetweenTimeValues.Add(TimeValue);
		BetweenIndexes.Add(-1);
	}
	if (EndTimeValue < LastTimeValue)
	{
		EndTimeValue += USMPFunctions::SECONDS_IN_DAY;
	}

	// Shift next day weather times by 24 hours for simpler calculations
	bool IsNextDay = false;
	int LastWeatherTime = -1;
	for (FWeatherTimeOfDayData& Weather : CurrentWeatherDatas)
	{
		int WeatherTime = USMPFunctions::TimecodeToSeconds(Weather.StartTime);
		if (IsNextDay)
		{
			Weather.StartTime.Hours += 24;
		}
		else if (WeatherTime < LastWeatherTime)
		{
			Weather.StartTime.Hours += 24;
			IsNextDay = true;
		}
		else
		{
			LastWeatherTime = WeatherTime;
		}
	}

	// Calculate insert indexes
	int StartIndex = -1;
	int EndIndex = -1;
	if (StartTimeValue < USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[CurrentWeatherDatas.Num() - 1].StartTime))
	{
		for (int i = 0; i < CurrentWeatherDatas.Num(); i++)
		{
			int WeatherTime = USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[i].StartTime);

			if (StartIndex == -1 && StartTimeValue < WeatherTime)
			{
				StartIndex = i;
			}
			for (int j = 0; j < BetweenIndexes.Num(); j++)
			{
				if (BetweenIndexes[j] == -1 && BetweenTimeValues[j] < WeatherTime)
				{
					BetweenIndexes[j] = i;
				}
			}
			if (EndIndex == -1 && EndTimeValue < WeatherTime)
			{
				EndIndex = i;
				break;
			}
		}
	}
	else
	{
		StartIndex = CurrentWeatherDatas.Num();
	}

	for (int i = 0; i < BetweenIndexes.Num(); i++)
	{
		if (BetweenIndexes[i] == -1)
		{
			BetweenIndexes[i] = StartIndex + i + 1;
		}
	}
	if (EndIndex == -1)
	{
		EndIndex = StartIndex + BetweenIndexes.Num() + 1;
	}

	// Remove weather between start and end or replace it with new weather, keeping sun position
	for (int i = StartIndex; i < EndIndex; i++)
	{
		if (i >= CurrentWeatherDatas.Num())
		{
			break;
		}
		if (i == EndIndex - 1
			&& EndTimeValue == USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[EndIndex - 1].StartTime))
		{
			break;
		}

		int WeatherStartTime = USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[i].StartTime);
		CurrentWeatherDatas.RemoveAt(i);
		CurrentWeatherTypes.RemoveAt(i);

		for (int j = 0; j < BetweenIndexes.Num(); j++)
		{
			if (BetweenTimeValues[j] >= WeatherStartTime)
			{
				BetweenIndexes[j] = BetweenIndexes[j] - 1;
			}
		}
		i -= 1;
		EndIndex -= 1;
	}

	// Reset next day weather to proper StartTime
	for (FWeatherTimeOfDayData& Weather : CurrentWeatherDatas)
	{
		if (Weather.StartTime.Hours >= 24)
		{
			Weather.StartTime.Hours -= 24;
		}
	}
	if (StartTimeValue >= USMPFunctions::SECONDS_IN_DAY)
	{
		StartTimeValue -= USMPFunctions::SECONDS_IN_DAY;
	}
	for (int i = 0; i < BetweenTimeValues.Num(); i++)
	{
		if (BetweenTimeValues[i] >= USMPFunctions::SECONDS_IN_DAY)
		{
			BetweenTimeValues[i] -= USMPFunctions::SECONDS_IN_DAY;
		}
	}
	if (EndTimeValue >= USMPFunctions::SECONDS_IN_DAY)
	{
		EndTimeValue -= USMPFunctions::SECONDS_IN_DAY;
	}

	// Generate proper weather at specified times
	FWeatherTimeOfDayData StartWeather;
	FString StartWeatherType;
	if (StartIndex >= CurrentWeatherDatas.Num())
	{
		if (AllowedWeathers.Contains(WeatherTypesToInsert[0]))
		{
			StartWeatherType = WeatherTypesToInsert[0];
		}
		else
		{
			StartWeatherType = CurrentWeatherTypes[CurrentWeatherTypes.Num() - 1] + "";
		}
		StartWeather = FindWeatherData(StartTime, StartWeatherType);
		StartWeather.StartTime = StartTime;
	}
	else
	{
		StartWeather = LerpWeather(CurrentWeatherDatas[StartIndex - 1], CurrentWeatherDatas[StartIndex], StartTime);
		StartWeatherType = CurrentWeatherTypes[StartIndex - 1] + "";
	}

	for (int i = 0; i < WeathersToInsert.Num(); i++)
	{
		if ((KeepSunMovement || i == 0 || i == WeathersToInsert.Num() - 1)
			&& BetweenIndexes[i] < CurrentWeatherDatas.Num())
		{
			FWeatherTimeOfDayData TempWeather = LerpWeather(CurrentWeatherDatas[BetweenIndexes[i] - 1], CurrentWeatherDatas[BetweenIndexes[i]], WeathersToInsert[i].StartTime);
			WeathersToInsert[i].SunLongitude = TempWeather.SunLongitude;
			WeathersToInsert[i].SunAltitude = TempWeather.SunAltitude;
		}
	}

	FWeatherTimeOfDayData EndWeather;
	FString EndWeatherType;
	if (EndIndex >= CurrentWeatherDatas.Num())
	{
		if (AllowedWeathers.Contains(WeatherTypesToInsert[WeatherTypesToInsert.Num() - 1]))
		{
			EndWeatherType = WeatherTypesToInsert[WeatherTypesToInsert.Num() - 1];
		}
		else
		{
			EndWeatherType = CurrentWeatherTypes[CurrentWeatherTypes.Num() - 1];
		}
		EndWeather = FindWeatherData(EndTime, EndWeatherType);
		EndWeather.StartTime = EndTime;
	}
	else
	{
		EndWeather = LerpWeather(CurrentWeatherDatas[EndIndex - 1], CurrentWeatherDatas[EndIndex], EndTime);
		if (EndIndex < CurrentWeatherTypes.Num()) {
			EndWeatherType = CurrentWeatherTypes[EndIndex] + "";
		}
		else
		{
			EndWeatherType = CurrentWeatherTypes[EndIndex - 1] + "";
		}
		
	}

	// Insert weather at proper location, replacing if time is exact
	if (StartIndex > CurrentWeatherDatas.Num()) {
		StartIndex = CurrentWeatherDatas.Num();
	}
	if (StartTimeValue == USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[StartIndex - 1].StartTime))
	{
		CurrentWeatherDatas.RemoveAt(StartIndex - 1);
		CurrentWeatherDatas.Insert(StartWeather, StartIndex - 1);
		CurrentWeatherTypes.RemoveAt(StartIndex - 1);
		CurrentWeatherTypes.Insert(StartWeatherType, StartIndex - 1);
	}
	else
	{
		CurrentWeatherDatas.Insert(StartWeather, StartIndex);
		CurrentWeatherTypes.Insert(StartWeatherType, StartIndex);
		for (int i = 0; i < BetweenIndexes.Num(); i++)
		{
			BetweenIndexes[i] = BetweenIndexes[i] + 1;
		}
		EndIndex += 1;
	}

	for (int i = 0; i < BetweenIndexes.Num(); i++)
	{
		if (BetweenIndexes[i] > CurrentWeatherDatas.Num())
		{
			BetweenIndexes[i] = CurrentWeatherDatas.Num();
		}
		if (BetweenTimeValues[i] == USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[BetweenIndexes[i] - 1].StartTime))
		{
			CurrentWeatherDatas.RemoveAt(BetweenIndexes[i] - 1);
			CurrentWeatherDatas.Insert(WeathersToInsert[i], BetweenIndexes[i] - 1);
			CurrentWeatherTypes.RemoveAt(BetweenIndexes[i] - 1);
			CurrentWeatherTypes.Insert(WeatherTypesToInsert[i], BetweenIndexes[i] - 1);
		}
		else
		{
			CurrentWeatherDatas.Insert(WeathersToInsert[i], BetweenIndexes[i]);
			CurrentWeatherTypes.Insert(WeatherTypesToInsert[i], BetweenIndexes[i]);
			for (int j = i; j < BetweenIndexes.Num(); j++)
			{
				BetweenIndexes[j] = BetweenIndexes[j] + 1;
			}
			EndIndex += 1;
		}
	}

	if (EndIndex > CurrentWeatherDatas.Num()) {
		EndIndex = CurrentWeatherDatas.Num();
	}
	if (EndTimeValue == USMPFunctions::TimecodeToSeconds(CurrentWeatherDatas[EndIndex - 1].StartTime))
	{
		CurrentWeatherDatas.RemoveAt(EndIndex - 1);
		CurrentWeatherDatas.Insert(EndWeather, EndIndex - 1);
		CurrentWeatherTypes.RemoveAt(EndIndex - 1);
		CurrentWeatherTypes.Insert(EndWeatherType, EndIndex - 1);
	}
	else
	{
		CurrentWeatherDatas.Insert(EndWeather, EndIndex);
		CurrentWeatherTypes.Insert(EndWeatherType, EndIndex);
	}

	ValidateAndFixCurrentWeatherDatas();

	CurrentWeather = CurrentWeatherDatas[0];
	CurrentWeatherType = CurrentWeatherTypes[0];
	NextWeather = CurrentWeatherDatas[1];
	NextWeatherType = CurrentWeatherTypes[1];
}

FString AWeatherActor::CalculateNextWeatherType(FRandomStream &RandomStream, const TArray<FString> &TheAllowedWeathers)
{
	TArray<FNextPossibleWeather> NextPossibleWeathers = Weathers[NextWeatherType].NextPossibleWeathers
		.FilterByPredicate([&TheAllowedWeathers](const FNextPossibleWeather& NextPossibleWeather)
	{
		return TheAllowedWeathers.Contains(NextPossibleWeather.WeatherType);
	});

	if (NextPossibleWeathers.Num() > 0)
	{
		int TotalWeatherPossibilities = 0;
		for (FNextPossibleWeather NextPossibleWeather : NextPossibleWeathers)
		{
			TotalWeatherPossibilities += NextPossibleWeather.Possibility;
		}
		int NextPossibleWeatherRand = RandomStream.RandRange(0, TotalWeatherPossibilities);
		int PossibilityOffset = 0;
		for (FNextPossibleWeather NextPossibleWeather : NextPossibleWeathers)
		{
			PossibilityOffset += NextPossibleWeather.Possibility;
			if (NextPossibleWeatherRand < PossibilityOffset)
			{
				return NextPossibleWeather.WeatherType;
			}
		}
	}

	TArray<FString> WeatherTypes = TheAllowedWeathers;

	if (WeatherTypes.Num() <= 0)
	{
		Weathers.GetKeys(WeatherTypes);
	}

	return WeatherTypes[UKismetMathLibrary::RandomInteger(WeatherTypes.Num())];
}

void AWeatherActor::Multicast_PauseChangingWeather_Implementation()
{
	ChangingWeatherTimeline->Stop();
}

void AWeatherActor::Multicast_ResumeChangingWeather_Implementation()
{
	ChangingWeatherTimeline->Play();
}

void AWeatherActor::AttachRainToCamera(APlayerCameraManager* CameraManager)
{
	AttachedCamera = CameraManager;
}

float AWeatherActor::GetFogMaxOpacity()
{
	return ExponentialHeightFog->FogMaxOpacity;
}

void AWeatherActor::SetFogMaxOpacity(float Opacity)
{
	ExponentialHeightFog->SetFogMaxOpacity(Opacity);
}

TArray<FString> AWeatherActor::GetNormalWeatherTypes()
{
	TArray<FString> WeatherTypes;
	Weathers.GetKeys(WeatherTypes);
	return WeatherTypes;
}

TArray<FString> AWeatherActor::GetSpecialWeathersTypes()
{
	TArray<FString> WeatherTypes;
	SpecialWeathers.GetKeys(WeatherTypes);
	return WeatherTypes;
}

FWeatherTimeOfDayData AWeatherActor::FindWeatherData(FTimecode Time, FString WeatherType)
{
	FWeatherTimeOfDayData WeatherToReturn = Weathers[WeatherType].Hours[0];
	for (FWeatherTimeOfDayData WeatherTimeOfDayData : Weathers[WeatherType].Hours)
	{
		if (USMPFunctions::TimecodeToSeconds(WeatherTimeOfDayData.StartTime) <= USMPFunctions::TimecodeToSeconds(Time)
			&& USMPFunctions::TimecodeToSeconds(WeatherToReturn.StartTime) <= USMPFunctions::TimecodeToSeconds(WeatherTimeOfDayData.StartTime))
		{
			WeatherToReturn = WeatherTimeOfDayData;
		}
	}

	return WeatherToReturn;
}

FWeatherTimeOfDayData AWeatherActor::FindNextWeatherData(FTimecode Time, FString WeatherType)
{
	FWeatherTimeOfDayData WeatherToReturn;
	for (int i = 0; i < Weathers[WeatherType].Hours.Num(); i++)
	{
		if (USMPFunctions::TimecodeToSeconds(Weathers[WeatherType].Hours[i].StartTime) <= USMPFunctions::TimecodeToSeconds(Time))
		{
			if (i == Weathers[WeatherType].Hours.Num() - 1)
			{
				WeatherToReturn = Weathers[WeatherType].Hours[0];
			}
			else
			{
				WeatherToReturn = Weathers[WeatherType].Hours[i + 1];
			}
		}
	}

	return WeatherToReturn;
}

FWeatherTimeOfDayData AWeatherActor::LerpWeather(FWeatherTimeOfDayData& WeatherBefore, FWeatherTimeOfDayData& WeatherAfter, FTimecode Time)
{
	return LerpWeatherF(WeatherBefore, WeatherAfter, USMPFunctions::TimecodeToSeconds(Time));
}

FWeatherTimeOfDayData AWeatherActor::LerpWeatherF(FWeatherTimeOfDayData& WeatherBefore, FWeatherTimeOfDayData& WeatherAfter, float Time)
{
	WeatherLerpValue = CalculateWeatherLerpF(WeatherBefore.StartTime, WeatherAfter.StartTime, Time);

	FWeatherTimeOfDayData ReturnWeather;

	ReturnWeather.StartTime = USMPFunctions::SecondsToTimecode(Time);

	ReturnWeather.SunLongitude = UKismetMathLibrary::Lerp(WeatherBefore.SunLongitude, WeatherAfter.SunLongitude, WeatherLerpValue);
	ReturnWeather.SunAltitude = UKismetMathLibrary::Lerp(WeatherBefore.SunAltitude, WeatherAfter.SunAltitude, WeatherLerpValue);
	ReturnWeather.SunLightColor = UKismetMathLibrary::LinearColorLerp(WeatherBefore.SunLightColor, WeatherAfter.SunLightColor, WeatherLerpValue);
	ReturnWeather.SunLightIntensity = UKismetMathLibrary::Lerp(WeatherBefore.SunLightIntensity, WeatherAfter.SunLightIntensity, WeatherLerpValue);

	if (WeatherLerpValue < 0.5)
	{
		ReturnWeather.SkyCubemap = WeatherBefore.SkyCubemap;
	}
	else
	{
		ReturnWeather.SkyCubemap = WeatherAfter.SkyCubemap;
	}
	ReturnWeather.SkyColor = UKismetMathLibrary::LinearColorLerp(WeatherBefore.SkyColor, WeatherAfter.SkyColor, WeatherLerpValue);
	ReturnWeather.SkyLightColor = UKismetMathLibrary::LinearColorLerp(WeatherBefore.SkyLightColor, WeatherAfter.SkyLightColor, WeatherLerpValue);
	ReturnWeather.SkyLightIntensity = UKismetMathLibrary::Lerp(WeatherBefore.SkyLightIntensity, WeatherAfter.SkyLightIntensity, WeatherLerpValue);

	ReturnWeather.FogColor = UKismetMathLibrary::LinearColorLerp(WeatherBefore.FogColor, WeatherAfter.FogColor, WeatherLerpValue);
	ReturnWeather.FogDensity = UKismetMathLibrary::Lerp(WeatherBefore.FogDensity, WeatherAfter.FogDensity, WeatherLerpValue);
	ReturnWeather.FogHeightFalloff = UKismetMathLibrary::Lerp(WeatherBefore.FogHeightFalloff, WeatherAfter.FogHeightFalloff, WeatherLerpValue);

	ReturnWeather.RainColor = UKismetMathLibrary::LinearColorLerp(WeatherBefore.RainColor, WeatherAfter.RainColor, WeatherLerpValue);
	ReturnWeather.RainIntensity = UKismetMathLibrary::Lerp(WeatherBefore.RainIntensity, WeatherAfter.RainIntensity, WeatherLerpValue);
	ReturnWeather.RainVelocity = UKismetMathLibrary::VLerp(WeatherBefore.RainVelocity, WeatherAfter.RainVelocity, WeatherLerpValue);

	ReturnWeather.Wetness = UKismetMathLibrary::Lerp(WeatherBefore.Wetness, WeatherAfter.Wetness, WeatherLerpValue);
	ReturnWeather.WindIntensity = UKismetMathLibrary::Lerp(WeatherBefore.WindIntensity, WeatherAfter.WindIntensity, WeatherLerpValue);

	return ReturnWeather;
}

float AWeatherActor::CalculateWeatherLerp(FTimecode StartTime, FTimecode EndTime, FTimecode Time)
{
	return CalculateWeatherLerpF(StartTime, EndTime, USMPFunctions::TimecodeToSeconds(Time));
}

float AWeatherActor::CalculateWeatherLerpF(FTimecode StartTime, FTimecode EndTime, float Time)
{
	int CurrentWeatherTime = USMPFunctions::TimecodeToSeconds(StartTime);
	int NextWeatherTime = USMPFunctions::TimecodeToSeconds(EndTime);
	float CurrentTimeOffset = Time - CurrentWeatherTime;
	if (CurrentTimeOffset < 0) {
		CurrentTimeOffset += USMPFunctions::SECONDS_IN_DAY;
	}
	if (CurrentWeatherTime > NextWeatherTime) {
		NextWeatherTime += USMPFunctions::SECONDS_IN_DAY;
	}
	float WeatherTimeDiff = NextWeatherTime - CurrentWeatherTime;
	return WeatherTimeDiff == 0 ? 0 : CurrentTimeOffset / WeatherTimeDiff;
}

void AWeatherActor::ValidateAndFixTimecode(FTimecode& Timecode)
{
	while (Timecode.Seconds >= 60)
	{
		Timecode.Minutes += 1;
		Timecode.Seconds -= 60;
	}
	while (Timecode.Minutes >= 60)
	{
		Timecode.Hours += 1;
		Timecode.Minutes -= 60;
	}
	while (Timecode.Hours >= 24)
	{
		Timecode.Hours -= 24;
	}
}

void AWeatherActor::ValidateAndFixCurrentWeatherDatas()
{
	for (FWeatherTimeOfDayData& Weather : CurrentWeatherDatas)
	{
		ValidateAndFixTimecode(Weather.StartTime);
	}

	if (!HasAuthority())
	{
		for (int i = 0; i < CurrentWeatherDatas.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, CurrentWeatherDatas[i].StartTime.ToString() + " - " + CurrentWeatherTypes[i]);
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "-----");
	}
}