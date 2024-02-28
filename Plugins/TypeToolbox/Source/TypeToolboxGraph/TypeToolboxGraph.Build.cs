// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TypeToolboxGraph : ModuleRules
{
	public TypeToolboxGraph(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"BlueprintGraph",
			"Kismet",
			"KismetCompiler",
			"KismetWidgets",
			"TypeToolbox",
			"TypeToolboxEditor"
		});
	}
}