// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "BaseItem.generated.h"

class APlayerCharacter;
class UInventoryComponent;
class UInventoryUI;
class UItemWidget;
class UTooltipWidget;


UCLASS()
class STALKERMP_API ABaseItem : public AActor
{
	GENERATED_BODY()




// ------------------------------------------------------------------------- Function Overrides --------------------------------------------------------------------------

public:	
	// Sets default values for this actor's properties
	ABaseItem();

	virtual void BeginPlay() override;




// ------------------------------------------------------------------------- Properties --------------------------------------------------------------------------

private:

	UPROPERTY()
		UItemWidget* ItemWidget;


protected:

	UPROPERTY(BlueprintReadOnly, Replicated)
		UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadOnly)
		UInventoryComponent* InventoryComponentForPickup;

	UPROPERTY(BlueprintReadOnly)
		ADeadBodyBackpack* DeadBodyBackpack;

	UPROPERTY(BlueprintReadOnly, Replicated)
		APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FIntPoint InventorySize = FIntPoint(1, 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool CanBeUsed = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LongUseTime = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
		FText ItemDetails;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* IconImageTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UItemWidget> ItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundBase* UseSound;


public:

	UPROPERTY(ReplicatedUsing = OnRep_CanBePickedUp)
		bool CanBePickedUp = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UTooltipWidget> TooltipWidgetClass;


// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

protected:

	UFUNCTION()
		virtual void FillItemDetails();

	UFUNCTION()
		virtual void BeforeUse();

	UFUNCTION()
		virtual void AfterUse();


public:

	UFUNCTION()
		FText GetItemName() { return ItemName; }

	UFUNCTION()
		FText GetItemDetails() { return ItemDetails; }

	UFUNCTION()
		void SetInventoryComponent(UInventoryComponent* TheInventoryComponent) { InventoryComponent = TheInventoryComponent; }

	UFUNCTION()
		UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

	UFUNCTION()
		void SetInventoryComponentForPickup(UInventoryComponent* TheInventoryComponent) { InventoryComponentForPickup = TheInventoryComponent; }

	UFUNCTION()
		void SetDeadBodyBackpack(ADeadBodyBackpack* TheDeadBodyBackpack) { DeadBodyBackpack = TheDeadBodyBackpack; }

	UFUNCTION()
		ADeadBodyBackpack* GetDeadBodyBackpack() { return DeadBodyBackpack; }

	UFUNCTION()
		virtual void SetPlayerCharacter(APlayerCharacter* ThePlayerCharacter) { PlayerCharacter = ThePlayerCharacter; }

	UFUNCTION()
		virtual void OnPossess();

	UFUNCTION()
		void CreateItemWidget(UInventoryUI* InventoryUI);

	UFUNCTION()
		void RemoveItemWidget();

	UFUNCTION()
		UTexture2D* GetIconImageTexture() { return IconImageTexture; }

	UFUNCTION()
		UItemWidget* GetItemWidget() { return ItemWidget; }

	UFUNCTION()
		FIntPoint GetInventorySize() { return InventorySize; }

	UFUNCTION()
		bool IsCanBeUsed() { return CanBeUsed; }

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Use();

	UFUNCTION()
		void Pickup();

	UFUNCTION(NetMulticast, Reliable)
		virtual void Multicast_SetPosition(FVector NewLocation, FRotator NewRotation);

	UFUNCTION()
		void SetPickable(bool IsPickable);

	UFUNCTION()
		virtual void OnRep_CanBePickedUp();

	UFUNCTION(BlueprintImplementableEvent)
		FTransform GetSpawnTransformOffset();
	
};
