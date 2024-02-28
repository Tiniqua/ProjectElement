// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TypeToolbox : ModuleRules
{
	public TypeToolbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.AddRange(new string[]
		{
			"TypeToolbox/"
		});

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine",
			"DeveloperSettings",
			"AssetRegistry"
		});

		// Editor settings for TypeToolboxSettings engine notifications.
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
				"Slate",
				"SlateCore"
			});
		}
	}
}

 