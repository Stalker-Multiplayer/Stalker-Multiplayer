// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseStackableItem.h"
#include "Net/UnrealNetwork.h"



void ABaseStackableItem::FillItemDetails()
{
	Super::FillItemDetails();

	FFormatNamedArguments Args;
	Args.Add("MaxStackSize", MaxStackSize);
	ItemDetails = FText::Format(ItemDetails, Args);
}

void ABaseStackableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABaseStackableItem, StackSize, COND_InitialOrOwner);
}

void ABaseStackableItem::Multicast_ForceUpdateStackSize_Implementation(int TheStackSize)
{
	StackSize = TheStackSize;
}

int ABaseStackableItem::AddToStack(int AddAmount)
{
	int CanAddAmount = MaxStackSize - StackSize;
	if (CanAddAmount > AddAmount)
	{
		CanAddAmount = AddAmount;
	}

	StackSize += CanAddAmount;

	return CanAddAmount;
}

int ABaseStackableItem::RemoveFromStack(int RemoveAmount)
{
	int CanRemoveAmount = StackSize;
	if (CanRemoveAmount > RemoveAmount)
	{
		CanRemoveAmount = RemoveAmount;
	}

	StackSize -= CanRemoveAmount;

	return CanRemoveAmount;
}
