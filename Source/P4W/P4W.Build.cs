// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class P4W : ModuleRules
{
	public P4W(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("P4W");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "OnlineSubsystemSteam", "OnlineSubsystem" });
	}
}
