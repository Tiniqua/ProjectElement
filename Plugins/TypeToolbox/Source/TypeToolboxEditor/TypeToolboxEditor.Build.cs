// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class TypeToolboxEditor : ModuleRules
{
	public TypeToolboxEditor(ReadOnlyTargetRules Target) : base(Target)
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
			"InputCore",
			"PropertyEditor",
			"BlueprintGraph",
			"StructViewer",
			"LevelEditor",
			"MainFrame",
			"Kismet",
			"KismetCompiler",
			"KismetWidgets",
			"GraphEditor",
			"TypeToolbox",
			"TypeToolboxGraph"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"StructViewer", 
			"EditorStyle"
		});
	}
}
