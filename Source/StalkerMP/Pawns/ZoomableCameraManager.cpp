// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoomableCameraManager.h"

#include "StalkerMP/StalkerMPGameInstance.h"
#include "StalkerMP/Levels/WeatherActor.h"
#include "StalkerMP/GameModes/BasePlayerController.h"

#include "EngineUtils.h"

/*AZoomableCameraManager::AZoomableCameraManager()
{
	//ViewTargetTransitionParamsOverride.BlendTime = 1;
	//ViewTargetTransitionParamsOverride.bLockOutgoing = true;
}*/

void AZoomableCameraManager::BeginPlay()
{
	OnIntSettingUpdated(ABasePlayerController::FIELD_OF_VIEW_SETTING_KEY, GetGameInstance<UStalkerMPGameInstance>()
		->GetIntSettingValue(ESettingsType::Video, ABasePlayerController::FIELD_OF_VIEW_SETTING_KEY, ABasePlayerController::DEFAULT_FIELD_OF_VIEW));

	OnIntSettingValueUpdatedDelegate.BindUFunction(this, "OnIntSettingUpdated");
	UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.AddUnique(OnIntSettingValueUpdatedDelegate);

	for (TActorIterator<AWeatherActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AWeatherActor* WeatherActor = *ActorItr;
		WeatherActor->AttachRainToCamera(this);
		break;
	}
}

void AZoomableCameraManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UStalkerMPGameInstance::OnIntSettingValueUpdatedDelegate.Remove(OnIntSettingValueUpdatedDelegate);
}

void AZoomableCameraManager::UpdateCamera(float DeltaTime)
{
	if (GetFOVAngle() != TargetFieldOfView)
	{
		float Diff = GetFOVAngle() - TargetFieldOfView;
		float DiffChange = ZoomSpeed * DeltaTime;
		if (Diff <= DiffChange && Diff >= -DiffChange)
		{
			SetFOV(TargetFieldOfView);
		}
		else if (Diff > 0)
		{
			SetFOV(GetFOVAngle() - DiffChange);
		}
		else if (Diff < 0)
		{
			SetFOV(GetFOVAngle() + DiffChange);
		}
	}

	Super::UpdateCamera(DeltaTime);
}

/*void AZoomableCameraManager::SetViewTarget(class AActor* NewTarget, struct FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewTarget, ViewTargetTransitionParamsOverride);
}*/

void AZoomableCameraManager::OnIntSettingUpdated(FString Key, int Value)
{
	if (Key.Equals(ABasePlayerController::FIELD_OF_VIEW_SETTING_KEY))
	{
		BaseFieldOfView = Value;
		TargetFieldOfView = BaseFieldOfView / ZoomScale;
	}
}

void AZoomableCameraManager::SetZoomScale(float TheZoomScale)
{
	ZoomScale = TheZoomScale;
	TargetFieldOfView = BaseFieldOfView / ZoomScale;
}