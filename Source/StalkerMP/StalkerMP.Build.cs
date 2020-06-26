// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StalkerMP : ModuleRules
{
	public StalkerMP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNull" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}
