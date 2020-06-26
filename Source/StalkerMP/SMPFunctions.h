// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Misc/Timecode.h"

#include "SMPFunctions.generated.h"


/**
 * 
 */
UCLASS()
class STALKERMP_API USMPFunctions : public UObject
{
	GENERATED_BODY()

// ------------------------------------------------------------------------- Constants --------------------------------------------------------------------------

public:

	static const int SECONDS_IN_HOUR = 3600;
	static const int SECONDS_IN_DAY = SECONDS_IN_HOUR * 24;




// ------------------------------------------------------------------------- Functions --------------------------------------------------------------------------

public:

	UFUNCTION()
		static FTimecode ParseTimeString(FString TimeString);

	UFUNCTION()
		static FTimecode SecondsToTimecode(int SecondsVal);

	UFUNCTION()
		static int TimecodeToSeconds(FTimecode Timecode);

};
