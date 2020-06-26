// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ZoomableCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class STALKERMP_API AZoomableCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()





// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:

	//AZoomableCameraManager();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void UpdateCamera(float DeltaTime) override;
	//virtual void SetViewTarget(class AActor* NewTarget, struct FViewTargetTransitionParams TransitionParams) override;


// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	FScriptDelegate OnIntSettingValueUpdatedDelegate;

	UPROPERTY()
		float BaseFieldOfView = 90;

	UPROPERTY()
		float TargetFieldOfView = BaseFieldOfView;


protected:

	UPROPERTY(BlueprintReadOnly)
		float ZoomScale = 1.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ZoomSpeed = 200;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//	FViewTargetTransitionParams ViewTargetTransitionParamsOverride;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION()
		void OnIntSettingUpdated(FString Key, int Value);


public:

	UFUNCTION()
		void SetZoomScale(float TheZoomScale);

};
