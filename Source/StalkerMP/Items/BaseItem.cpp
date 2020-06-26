// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "StalkerMP/Items/InventoryComponent.h"
#include "StalkerMP/Items/HUD/InventoryUI.h"
#include "StalkerMP/Items/HUD/ItemWidget.h"
#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"



// ------------------------------------------------------------------------- Engine override --------------------------------------------------------------------------

ABaseItem::ABaseItem()
{
	NetCullDistanceSquared = 2500000000.0;

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;

	SetReplicates(true);
	//bReplicateMovement = true;
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	FillItemDetails();
}

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseItem, InventoryComponent);
	DOREPLIFETIME(ABaseItem, PlayerCharacter);
	DOREPLIFETIME(ABaseItem, CanBePickedUp);
}




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

void ABaseItem::FillItemDetails()
{
	// Do nothing
}

void ABaseItem::CreateItemWidget(UInventoryUI* InventoryUI)
{
	if (ItemWidget)
	{
		return;
	}

	ItemWidget = CreateWidget<UItemWidget>(InventoryUI, ItemWidgetClass.Get());
	ItemWidget->SetItem(this);
}

void ABaseItem::RemoveItemWidget()
{
	if (ItemWidget)
	{
		ItemWidget->RemoveFromParent();
		ItemWidget = nullptr;
	}
}

void ABaseItem::Server_Use_Implementation()
{
	if (CanBeUsed)
	{
		if (LongUseTime > 0)
		{
			FActionDelegate BeforeLongActionDelegate;
			BeforeLongActionDelegate.BindUFunction(this, FName("BeforeUse"));
			FActionDelegate AfterLongActionDelegate;
			AfterLongActionDelegate.BindUFunction(this, FName("AfterUse"));
			PlayerCharacter->DoLongAction(LongUseTime, BeforeLongActionDelegate, AfterLongActionDelegate, UseSound);
		}
		else
		{
			BeforeUse();
			AfterUse();
		}
	}
}

bool ABaseItem::Server_Use_Validate()
{
	return true;
}

void ABaseItem::BeforeUse()
{
	// do nothing
}

void ABaseItem::AfterUse()
{
	InventoryComponent->Server_OnItemUsed_Implementation(this);
}

void ABaseItem::Pickup()
{
	if (InventoryComponentForPickup)
	{
		InventoryComponentForPickup->Server_PickupItem(this);
	}
}

void ABaseItem::Multicast_SetPosition_Implementation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocationAndRotation(NewLocation, NewRotation, false, nullptr, ETeleportType::ResetPhysics);
}

void ABaseItem::OnPossess()
{}

void ABaseItem::SetPickable(bool IsPickable)
{
	CanBePickedUp = IsPickable;
}

void ABaseItem::OnRep_CanBePickedUp()
{}
