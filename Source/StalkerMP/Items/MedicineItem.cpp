// Fill out your copyright notice in the Description page of Project Settings.

#include "MedicineItem.h"

#include "StalkerMP/Pawns/PlayerCharacter/PlayerCharacter.h"


AMedicineItem::AMedicineItem()
{
	CanBeUsed = true;
}

void AMedicineItem::FillItemDetails()
{
	Super::FillItemDetails();

	FFormatNamedArguments Args;

	Args.Add("HealAmount", HealAmount);

	ItemDetails = FText::Format(ItemDetails, Args);
}

void AMedicineItem::Server_Use_Implementation()
{
	if (PlayerCharacter->GetHealth() < PlayerCharacter->GetMaxHealth())
	{
		Super::Server_Use_Implementation();
	}
}

void AMedicineItem::BeforeUse()
{

}

void AMedicineItem::AfterUse()
{
	PlayerCharacter->Heal(HealAmount);
	Super::AfterUse();
}
