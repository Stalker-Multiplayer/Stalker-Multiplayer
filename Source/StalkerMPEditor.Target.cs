// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class StalkerMPEditorTarget : TargetRules
{
	public StalkerMPEditorTarget(TargetInfo Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.V2;

        Type = TargetType.Editor;
		ExtraModuleNames.Add("StalkerMP");

        bUsesSteam = true;
        //bUseUnityBuild = false;
    }
}
