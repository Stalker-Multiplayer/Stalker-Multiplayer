// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class StalkerMPServerTarget : TargetRules
{
    public StalkerMPServerTarget(TargetInfo Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.V2;

        Type = TargetType.Server;
        ExtraModuleNames.Add("StalkerMP");

        bUsesSteam = true;
        //bUseUnityBuild = false;
    }
}