// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spectator.generated.h"

class USphereComponent;
class UFloatingPawnMovement;


UCLASS()
class STALKERMP_API ASpectator : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpectator();



// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

protected:

	// Components

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UFloatingPawnMovement* FloatingPawnMovement;


	// Movement

	UPROPERTY()
		bool bSprintActionPressed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float RunningSpeed = 1800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float SprintingSpeed = 3600;





// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

private:

	// Movement

	UFUNCTION()
		void UpdateMovingMode();

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void MoveUp(float Value);

	UFUNCTION()
		void Sprint();

	UFUNCTION()
		void StopSprinting();

};
