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

	DOREPLIFETIME_CONDITION(AWeatherActor, CurrentTime, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, TimelineLength, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, ChangingTimeRichCurve, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, CurrentWeatherDatas, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeatherActor, CurrentWeatherNames, COND_InitialOnly);
}

void AWeatherActor::BeginPlay()
{
	Super::BeginPlay();

	ChangingWeatherTimeline = NewObject<UTimelineComponent>(this, FName("WeatherTimeline"));
	ChangingWeatherTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	this->BlueprintCreatedComponents.Add(ChangingWeatherTimeline); // Add to array so it gets saved
	//ChangingWeatherTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication
	//ChangingWeatherTimeline->SetIsReplicated(true);
	ChangingWeatherTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	ChangingWeatherTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
	ChangingWeatherTimeline->SetLooping(false);
	ChangingWeatherTimeline->RegisterComponent();

	ChangingTimeCurve = NewObject<UCurveFloat>();
	FOnTimelineFloat onTimeChangeCallback;
	onTimeChangeCallback.BindUFunction(this, FName{ TEXT("ApplyCurrentWeatherTimeline") });
	ChangingWeatherTimeline->AddInterpFloat(ChangingTimeCurve, onTimeChangeCallback);

	TArray<FString> WeatherNames;
	WeatherTypes.GetKeys(WeatherNames);
	AllowedWeathers = WeatherNames;
	CurrentWeatherName = WeatherNames[0];
	NextWeatherName = WeatherNames[0];

	if (HasAuthority())
	{
		RainParticleSystem->Deactivate();
		RainDepthCaptureComponent2D->Deactivate();

		CurrentWeatherDatas = WeatherTypes[CurrentWeatherName].Hours;
		for (int i = 0; i < CurrentWeatherDatas.Num(); i++)
		{
			CurrentWeatherNames.Add(CurrentWeatherName);
		}

		int Hour = UKismetMathLibrary::RandomInteger(24);
		int Minute = UKismetMathLibrary::RandomInteger(60);
		FTimecode Time = FTimecode(Hour, Minute, 0, 0, false);
		CurrentWeather = FindWeatherData(Time, CurrentWeatherName);
		NextWeather = FindNextWeatherData(Time, CurrentWeatherName);
		SetTimeOfDay(Time, 0, false);
	}
	else
	{
		ChangingWeatherTimeline->SetTimelineLength(TimelineLength);

		if (ChangingTimeRichCurve.Keys.Num() > 0)
		{
			ChangingTimeCurve->FloatCurve = ChangingTimeRichCurve;
			OnEverythingReplicated();
		}
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

void AWeatherActor::OnRep_CurrentTime()
{
	CurrentTimeReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_TimelineLength()
{
	if (ChangingWeatherTimeline)
	{
		ChangingWeatherTimeline->SetTimelineLength(TimelineLength);
	}
	TimelineLengthReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_ChangingTimeRichCurve()
{
	if (ChangingTimeCurve)
	{
		ChangingTimeCurve->FloatCurve = ChangingTimeRichCurve;
	}
	ChangingTimeRichCurveReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_CurrentWeatherDatas()
{
	CurrentWeatherDatasReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnRep_CurrentWeatherNames()
{
	CurrentWeatherNamesReplicated = true;
	OnEverythingReplicated();
}

void AWeatherActor::OnEverythingReplicated()
{
	if (CurrentTimeReplicated && TimelineLengthReplicated && ChangingTimeRichCurveReplicated && CurrentWeatherDatasReplicated && CurrentWeatherNamesReplicated)
	{
		CurrentWeather = CurrentWeatherDatas[0];
		CurrentWeatherName = CurrentWeatherNames[0];
		NextWeather = CurrentWeatherDatas[1];
		NextWeatherName = CurrentWeatherNames[1];
		WeatherDataIsReady = true;
		ApplyCurrentWeatherTimeline(CurrentTime);
		ChangingWeatherTimeline->Play();
	}
}

void AWeatherActor::ApplyCurrentWeatherTimeline(float Val)
{
	if (!WeatherDataIsReady || CurrentWeatherDatas.Num() < 2)
	{
		return;
	}

	while (CurrentWeatherDatas.Num() >= 2
		&& (
			(USMPFunctions::TimecodeToSeconds(NextWeather.StartTime) <= Val
				&& USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) < USMPFunctions::TimecodeToSeconds(NextWeather.StartTime))
			||
			(CurrentTime > Val
				&& USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) > USMPFunctions::TimecodeToSeconds(NextWeather.StartTime)))
		)
	{
		if (CurrentWeatherDatas.Num() > 2)
		{
			CurrentWeatherDatas.RemoveAt(0);
			CurrentWeatherNames.RemoveAt(0);
		}

		CurrentWeather = CurrentWeatherDatas[0];
		CurrentWeatherName = CurrentWeatherNames[0];
		NextWeather = CurrentWeatherDatas[1];
		NextWeatherName = CurrentWeatherNames[1];

		CurrentTime = Val;

		if (CurrentWeatherDatas.Num() <= 2)
		{
			break;
		}
	}

	CurrentTime = Val;

	if (!HasAuthority())
	{
		Update(USMPFunctions::SecondsToTimecode(Val), CurrentWeather, NextWeather);
	}
}

void AWeatherActor::Update(FTimecode Time, FWeatherTimeOfDayData WeatherData, FWeatherTimeOfDayData NextWeatherData)
{
	int NextWeatherTime = USMPFunctions::TimecodeToSeconds(NextWeatherData.StartTime);
	if (USMPFunctions::TimecodeToSeconds(WeatherData.StartTime) > USMPFunctions::TimecodeToSeconds(NextWeatherData.StartTime)) {
		NextWeatherTime += USMPFunctions::SECONDS_IN_DAY;
	}
	float CurrentTimeOffset = USMPFunctions::TimecodeToSeconds(Time) - USMPFunctions::TimecodeToSeconds(WeatherData.StartTime);
	float WeatherTimeDiff = NextWeatherTime - USMPFunctions::TimecodeToSeconds(WeatherData.StartTime);
	WeatherLerpValue = WeatherTimeDiff == 0 ? 0 : CurrentTimeOffset / WeatherTimeDiff;

	FRotator SunRotation = FRotator(UKismetMathLibrary::Lerp(WeatherData.SunLongitude, NextWeatherData.SunLongitude, WeatherLerpValue), 0, 0);
	FRotator SunAltitudeRotation = FRotator(0, UKismetMathLibrary::Lerp(WeatherData.SunAltitude, NextWeatherData.SunAltitude, WeatherLerpValue), 0);

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
	SkyMaterialDynamic->SetVectorParameterValue(SunLightColorParameterName, UKismetMathLibrary::LinearColorLerp(WeatherData.SunLightColor, NextWeatherData.SunLightColor, WeatherLerpValue));
	SkyMaterialDynamic->SetVectorParameterValue(SkyColorParameterName, UKismetMathLibrary::LinearColorLerp(WeatherData.SkyColor, NextWeatherData.SkyColor, WeatherLerpValue));

	float Wetness = UKismetMathLibrary::Lerp(WeatherData.Wetness, NextWeatherData.Wetness, WeatherLerpValue);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, WetnessParameterName, Wetness);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, WindIntensityParameterName,
		UKismetMathLibrary::Lerp(WeatherData.WindIntensity, NextWeatherData.WindIntensity, WeatherLerpValue));
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MaterialParameterCollection, SunLightVectorParameterName, CurrentSunRotation.Vector());
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MaterialParameterCollection, SunLightColorParameterName, UKismetMathLibrary::LinearColorLerp(WeatherData.SunLightColor, NextWeatherData.SunLightColor, WeatherLerpValue));
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MaterialParameterCollection, SunLightIntensityParameterName,
		UKismetMathLibrary::Lerp(WeatherData.SunLightIntensity, NextWeatherData.SunLightIntensity, WeatherLerpValue));

	SunLight->SetWorldRotation(CurrentSunRotation);
	SunLight->SetIntensity(UKismetMathLibrary::Lerp(WeatherData.SunLightIntensity, NextWeatherData.SunLightIntensity, WeatherLerpValue));
	SunLight->SetLightColor(UKismetMathLibrary::LinearColorLerp(WeatherData.SunLightColor, NextWeatherData.SunLightColor, WeatherLerpValue));

	SkyLight->SetIntensity(UKismetMathLibrary::Lerp(WeatherData.SkyLightIntensity, NextWeatherData.SkyLightIntensity, WeatherLerpValue));
	SkyLight->SetLightColor(UKismetMathLibrary::LinearColorLerp(WeatherData.SkyLightColor, NextWeatherData.SkyLightColor, WeatherLerpValue));

	ExponentialHeightFog->SetFogDensity(UKismetMathLibrary::Lerp(WeatherData.FogDensity, NextWeatherData.FogDensity, WeatherLerpValue));
	ExponentialHeightFog->SetFogHeightFalloff(UKismetMathLibrary::Lerp(WeatherData.FogHeightFalloff, NextWeatherData.FogHeightFalloff, WeatherLerpValue));
	ExponentialHeightFog->SetFogInscatteringColor(UKismetMathLibrary::LinearColorLerp(WeatherData.FogColor, NextWeatherData.FogColor, WeatherLerpValue));

	int RainIntensity = UKismetMathLibrary::Lerp(WeatherData.RainIntensity, NextWeatherData.RainIntensity, WeatherLerpValue);
	RainParticleSystem->SetColorParameter(RainColorParameterName, UKismetMathLibrary::LinearColorLerp(WeatherData.RainColor, NextWeatherData.RainColor, WeatherLerpValue));
	RainParticleSystem->SetFloatParameter(RainIntensityParameterName, RainIntensity);
	RainParticleSystem->SetVectorParameter(RainVelocityParameterName, UKismetMathLibrary::VLerp(WeatherData.RainVelocity, NextWeatherData.RainVelocity, WeatherLerpValue));

	if (RainIntensity > 0 || Wetness > 0)
	{
		RainDepthCaptureComponent2D->Activate();
	}
	else
	{
		RainDepthCaptureComponent2D->Deactivate();
	}
}

void AWeatherActor::SetTimeOfDay(FTimecode FinalTime, int SecondsForChange, bool ForceNextDay)
{
	Multicast_GenerateWeather(FMath::Rand(), FinalTime, SecondsForChange, ForceNextDay, AllowedWeathers);
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

void AWeatherActor::Multicast_GenerateWeather_Implementation(int Seed, FTimecode FinalTime, int SecondsForChange, bool ForceNextDay, const TArray<FString> &TheAllowedWeathers)
{
	WeatherDataIsReady = false;

	ChangingWeatherTimeline->Stop();
	int FinalTimeInSeconds = USMPFunctions::TimecodeToSeconds(FinalTime);

	FRandomStream RandomStream = FRandomStream(Seed);

	float InitialPosition;
	bool WillBeNextDay = ForceNextDay || CurrentTime >= FinalTimeInSeconds;

	CurrentWeatherDatas.Empty();
	CurrentWeatherNames.Empty();

	ChangingTimeRichCurve = FRichCurve();
	if (SecondsForChange <= 0)
	{
		InitialPosition = 0;
		TimelineLength = 0;

		ChangingTimeRichCurve.AddKey(0, FinalTimeInSeconds);

		NextWeatherName = CalculateNextWeatherName(RandomStream, TheAllowedWeathers);
		CurrentWeatherDatas.Add(FindWeatherData(FinalTime, NextWeatherName));
		CurrentWeatherNames.Add(NextWeatherName);
		NextWeatherName = CalculateNextWeatherName(RandomStream, TheAllowedWeathers);
		CurrentWeatherDatas.Add(FindNextWeatherData(FinalTime, NextWeatherName));
		CurrentWeatherNames.Add(NextWeatherName);
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

			FKeyHandle ZeroKey = ChangingTimeRichCurve.AddKey(SecondsTillNextDay - 0.0001f, USMPFunctions::SECONDS_IN_DAY);
			if (FinalTimeInSeconds != 0)
			{
				ChangingTimeRichCurve.SetKeyInterpMode(ZeroKey, ERichCurveInterpMode::RCIM_Constant);
				ChangingTimeRichCurve.AddKey(InitialPosition + SecondsTillNextDay, 0);
			}
		}

		ChangingTimeRichCurve.AddKey(TimelineLength + InitialPosition, FinalTimeInSeconds);

		CurrentWeatherDatas.Add(CurrentWeather);
		CurrentWeatherNames.Add(CurrentWeatherName);
		CurrentWeatherDatas.Add(NextWeather);
		CurrentWeatherNames.Add(NextWeatherName);

		CurrentWeather = NextWeather;
		while (WillBeNextDay || USMPFunctions::TimecodeToSeconds(CurrentWeather.StartTime) < FinalTimeInSeconds)
		{
			NextWeatherName = CalculateNextWeatherName(RandomStream, TheAllowedWeathers);

			NextWeather = FindNextWeatherData(CurrentWeather.StartTime, NextWeatherName);
			CurrentWeatherDatas.Add(NextWeather);
			CurrentWeatherNames.Add(NextWeatherName);

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
	CurrentWeatherName = CurrentWeatherNames[0];
	NextWeather = CurrentWeatherDatas[1];
	NextWeatherName = CurrentWeatherNames[1];

	ChangingTimeCurve->FloatCurve = ChangingTimeRichCurve;
	ChangingWeatherTimeline->SetTimelineLength(TimelineLength);

	WeatherDataIsReady = true;
	ChangingWeatherTimeline->SetPlaybackPosition(InitialPosition, true, true);
	ChangingWeatherTimeline->Play();
}

FString AWeatherActor::CalculateNextWeatherName(FRandomStream &RandomStream, const TArray<FString> &TheAllowedWeathers)
{
	TArray<FNextPossibleWeather> NextPossibleWeathers = WeatherTypes[NextWeatherName].NextPossibleWeathers
		.FilterByPredicate([&TheAllowedWeathers](const FNextPossibleWeather& NextPossibleWeather)
	{
		return TheAllowedWeathers.Contains(NextPossibleWeather.WeatherName);
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
				return NextPossibleWeather.WeatherName;
			}
		}
	}

	TArray<FString> WeatherNames = TheAllowedWeathers;

	if (WeatherNames.Num() <= 0)
	{
		WeatherTypes.GetKeys(WeatherNames);
	}

	return WeatherNames[UKismetMathLibrary::RandomInteger(WeatherNames.Num())];
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

TArray<FString> AWeatherActor::GetAllWeatherNames()
{
	TArray<FString> WeatherNames;
	WeatherTypes.GetKeys(WeatherNames);
	return WeatherNames;
}

FWeatherTimeOfDayData AWeatherActor::FindWeatherData(FTimecode Time, FString WeatherType)
{
	FWeatherTimeOfDayData WeatherToReturn;
	for (FWeatherTimeOfDayData WeatherTimeOfDayData : WeatherTypes[WeatherType].Hours)
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
	for (int i = 0; i < WeatherTypes[WeatherType].Hours.Num(); i++)
	{
		if (USMPFunctions::TimecodeToSeconds(WeatherTypes[WeatherType].Hours[i].StartTime) <= USMPFunctions::TimecodeToSeconds(Time))
		{
			if (i == WeatherTypes[WeatherType].Hours.Num() - 1)
			{
				WeatherToReturn = WeatherTypes[WeatherType].Hours[0];
			}
			else
			{
				WeatherToReturn = WeatherTypes[WeatherType].Hours[i + 1];
			}
		}
	}

	return WeatherToReturn;
}