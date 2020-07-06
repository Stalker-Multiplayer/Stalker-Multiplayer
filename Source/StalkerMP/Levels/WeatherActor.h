// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Misc/Timecode.h"
#include "GameFramework/Actor.h"
#include "Curves/RichCurve.h"
#include "Components/TimelineComponent.h"

#include "WeatherActor.generated.h"

class UTextureCube;
class UStaticMeshComponent;
class USkyLightComponent;
class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class USceneCaptureComponent2D;
class UParticleSystemComponent;
class UMaterialParameterCollection;

USTRUCT(BlueprintType)
struct FWeatherTimeOfDayData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FTimecode StartTime = FTimecode(-1, -1, -1, -1, false);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTextureCube* SkyCubemap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SunLightIntensity = 2.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor SunLightColor = FLinearColor(1.0, 1.0, 1.0, 1.0);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SunLongitude = -50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SunAltitude = -115;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SkyLightIntensity = 20.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor SkyLightColor = FLinearColor(1.0, 1.0, 1.0, 1.0);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor SkyColor = FLinearColor(1.0, 1.0, 1.0, 1.0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float FogDensity = 0.01;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float FogHeightFalloff = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor FogColor = FLinearColor(0.60, 0.73, 1.00, 1.00);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Wetness = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float WindIntensity = 15.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FLinearColor RainColor = FLinearColor(0.68, 0.64, 0.60, 1.00);
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int RainIntensity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector RainVelocity = FVector(0, 0, -1000);
};

USTRUCT(BlueprintType)
struct FNextPossibleWeather
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString WeatherName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int Possibility = 10;

};

USTRUCT(BlueprintType)
struct FWeatherTypeData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FWeatherTimeOfDayData> Hours;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FNextPossibleWeather> NextPossibleWeathers;

};

UCLASS()
class STALKERMP_API AWeatherActor : public AActor
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:
	// Sets default values for this actor's properties
	AWeatherActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UMaterialInstanceDynamic* SkyMaterialDynamic;

	UPROPERTY()
		UTimelineComponent* ChangingWeatherTimeline;

	UPROPERTY()
		UCurveFloat* ChangingTimeCurve;

	UPROPERTY()
		FWeatherTimeOfDayData CurrentWeather;

	UPROPERTY()
		bool WeatherDataIsReady = false;

	UPROPERTY()
		float WeatherLerpValue = 0;

	UPROPERTY()
		FString CurrentWeatherName;

	UPROPERTY()
		FString NextWeatherName;

	UPROPERTY()
		FWeatherTimeOfDayData NextWeather;

	UPROPERTY()
		TArray<FString> AllowedWeathers;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentTime)
		int CurrentTime;

	UPROPERTY()
		bool CurrentTimeReplicated = false;

	UPROPERTY(ReplicatedUsing = OnRep_TimelineLength)
		int TimelineLength = -1;

	UPROPERTY()
		bool TimelineLengthReplicated = false;

	UPROPERTY(ReplicatedUsing = OnRep_ChangingTimeRichCurve)
		FRichCurve ChangingTimeRichCurve;

	UPROPERTY()
		bool ChangingTimeRichCurveReplicated = false;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeatherDatas)
		TArray<FWeatherTimeOfDayData> CurrentWeatherDatas;

	UPROPERTY()
		bool CurrentWeatherDatasReplicated = false;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeatherNames)
		TArray<FString> CurrentWeatherNames;

	UPROPERTY()
		bool CurrentWeatherNamesReplicated = false;

	UPROPERTY()
		APlayerCameraManager* AttachedCamera;


protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USceneComponent* DefaultComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* SkyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USkyLightComponent* SkyLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UDirectionalLightComponent* SunLight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UExponentialHeightFogComponent* ExponentialHeightFog;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UParticleSystemComponent* RainParticleSystem;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USceneCaptureComponent2D* RainDepthCaptureComponent2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UMaterialInterface* SkyMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UMaterialParameterCollection* MaterialParameterCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName CurrentSkyTextureParameterName = FName(TEXT("SkyCurrent"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName NextSkyTextureParameterName = FName(TEXT("SkyNext"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName SkyColorParameterName = FName(TEXT("SkyColor"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName SunLightColorParameterName = FName(TEXT("SunLightColor"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName SunLightIntensityParameterName = FName(TEXT("SunLightIntensity"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName LerpParameterName = FName(TEXT("SkyLerp"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName SunLightVectorParameterName = FName(TEXT("SunLightVector"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName WetnessParameterName = FName(TEXT("Wetness"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName WindIntensityParameterName = FName(TEXT("WindIntensity"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName RainColorParameterName = FName(TEXT("RainColor"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName RainIntensityParameterName = FName(TEXT("RainIntensity"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName RainVelocityParameterName = FName(TEXT("RainVelocity"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName RainEmitterLocationParameterName = FName(TEXT("RainEmitterLocation"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TMap<FString, FWeatherTypeData> WeatherTypes;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	UFUNCTION()
		void OnRep_CurrentTime();

	UFUNCTION()
		void OnRep_TimelineLength();

	UFUNCTION()
		void OnRep_ChangingTimeRichCurve();

	UFUNCTION()
		void OnRep_CurrentWeatherDatas();

	UFUNCTION()
		void OnRep_CurrentWeatherNames();

	UFUNCTION()
		void OnEverythingReplicated();

	UFUNCTION()
		void ApplyCurrentWeatherTimeline(float Val);

	UFUNCTION()
		FString CalculateNextWeatherName(FRandomStream &RandomStream, const TArray<FString> &TheAllowedWeathers);

	UFUNCTION()
		FWeatherTimeOfDayData FindWeatherData(FTimecode Time, FString WeatherType);

	UFUNCTION()
		FWeatherTimeOfDayData FindNextWeatherData(FTimecode Time, FString WeatherType);

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_GenerateWeather(int StartTime, int Seed, FTimecode FinalTime, int SecondsForChange, bool ForceNextDay, const TArray<FString> &TheAllowedWeathers);


protected:

	UFUNCTION(BlueprintCallable)
		void Update(FTimecode Time, FWeatherTimeOfDayData WeatherData, FWeatherTimeOfDayData NextWeatherData);


public:

	UFUNCTION()
		FString GetCurrentWeatherName() { return CurrentWeatherName; }

	UFUNCTION()
		FString GetNextWeatherName() { return NextWeatherName; }

	UFUNCTION()
		float GetWeatherLerpValue() { return WeatherLerpValue; }

	UFUNCTION()
		float GetFogMaxOpacity();

	UFUNCTION()
		void SetFogMaxOpacity(float Opacity);

	UFUNCTION()
		TArray<FString> GetAllWeatherNames();

	UFUNCTION()
		void SetAllowedWeathers(const TArray<FString> &TheAllowedWeathers) { AllowedWeathers = TheAllowedWeathers; };

	UFUNCTION()
		void SetTimeOfDay(FTimecode FinalTime, int SecondsForChange, bool ForceNextDay);

	UFUNCTION()
		void AddTime(FTimecode Time, int SecondsForChange);

	UFUNCTION()
		int GetCurrentTime() { return CurrentTime; }

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_PauseChangingWeather();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_ResumeChangingWeather();

	UFUNCTION()
		void AttachRainToCamera(APlayerCameraManager* CameraManager);


};
