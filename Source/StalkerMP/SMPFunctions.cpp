// Fill out your copyright notice in the Description page of Project Settings.


#include "SMPFunctions.h"

FTimecode USMPFunctions::ParseTimeString(FString TimeString)
{
	TArray<FString> Array;
	TimeString.ParseIntoArray(Array, TEXT(":"), true);

	int Hour = (Array.Num() >= 1) ? FCString::Atoi(*Array[0]) : 0;
	int Minute = (Array.Num() >= 2) ? FCString::Atoi(*Array[1]) : 0;
	int Second = (Array.Num() >= 3) ? FCString::Atoi(*Array[2]) : 0;

	return FTimecode(Hour, Minute, Second, 0, false);
}

FTimecode USMPFunctions::SecondsToTimecode(int SecondsVal)
{
	int Hours = SecondsVal / SECONDS_IN_HOUR;
	int Minutes = (SecondsVal - Hours * SECONDS_IN_HOUR) / 60;
	int Seconds = (SecondsVal - Hours * SECONDS_IN_HOUR - Minutes * 60);
	return FTimecode(Hours, Minutes, Seconds, 0, false);
}

int USMPFunctions::TimecodeToSeconds(FTimecode Timecode)
{
	return Timecode.Hours * SECONDS_IN_HOUR + Timecode.Minutes * 60 + Timecode.Seconds;
}
