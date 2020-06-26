// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorItem.h"

#include "StalkerMP/DataClasses/GlobalEnums.h"
#include "StalkerMP/Items/InventoryComponent.h"


AArmorItem::AArmorItem()
{
	CanBeUsed = true;
}

void AArmorItem::FillItemDetails()
{
	Super::FillItemDetails();

	FFormatNamedArguments Args;

	Args.Add("BulletDamageReduction", BulletDamageReduction * 100);
	Args.Add("IncludesHelmet", IncludesHelmet ? FText::FromString("Has helmet") : FText::FromString("Doesn't have helmet"));
	Args.Add("FallVelocityReduction", FallVelocityReduction);

	ItemDetails = FText::Format(ItemDetails, Args);
}

float AArmorItem::GetBulletDamageReduction(EBodyPart BodyPart) {
	if (BodyPart != EBodyPart::Head || IncludesHelmet)
	{
		return BulletDamageReduction;
	}

	return 0;
}
