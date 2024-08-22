// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RunNekoRun : ModuleRules
{
	public RunNekoRun(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "HeadMountedDisplay", 
            "EnhancedInput", 
            "Landscape",
            "AIModule",
            "NavigationSystem",
            "GameplayTasks"
        });
        bEnableExceptions = true;
    }
}
