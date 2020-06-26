// Fill out your copyright notice in the Description page of Project Settings.

#include "ZoneDamageType.h"



UZoneDamageType::UZoneDamageType()
{
	bCausedByWorld = true;
	bScaleMomentumByMass = false;
	DamageImpulse = 0;
	DestructibleImpulse = 0;
}
