// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectElement : ModuleRules
{
	public ProjectElement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Avoids having to include via ../
		PrivateIncludePaths.Add( "ProjectElement/" );

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"EnhancedInput",
			"NavigationSystem",
			"AIModule",
			"Niagara",
			"UMG",
			"EOSCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"Slate", 
			"SlateCore", 
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"EOSCore",
			"Voice"
		});
		
		// AI modules
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTasks",
			"GameplayTags",
			"GameplayAbilities"
		});
		
		// Imgui implementation.
		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PrivateDependencyModuleNames.Add("ImGui");
		}
    }
}
