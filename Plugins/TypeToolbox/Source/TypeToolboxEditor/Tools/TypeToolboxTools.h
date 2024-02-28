// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/** Toolbox tools submenu implementations for editor tooling. */
class FTypeToolboxTools
{
	
public:
	
	virtual ~FTypeToolboxTools() {}

	/** Create command list, map commands and add them to the menu extender. */
	void StartupTools();

	/** Unregister commands. */
	void ShutdownTools();

	/** Type Toolbox editor settings menu. */
	void MakeTypeToolboxMenu(FMenuBarBuilder& InMenuBuilder);
	void PopulateTypeToolboxMenu(FMenuBuilder& InMenuBuilder) const;

protected:

	/** Pointer to the LevelEditorMenu top bar menu in the editor. */
	TSharedPtr<FExtensibilityManager> LevelEditorMenuExtensibilityManager;

	/** Custom command tools menu extender for Type Toolbox commands. */
	TSharedPtr<FExtender> TypeToolsMenuExtender;
	
	/** List of commands. */
	TSharedPtr<FUICommandList> CommandList = nullptr;

	/** Toolbox Data type status window ptr. */
	TSharedPtr<SWindow> DataTypeStatusWindow = nullptr;

#pragma region Command Functions

	/** Reload the data type subsystem and the data types / tables. */
	void RefreshDataTypes() const;

	/** Opens data types status window and shows the load success of each data type loaded in the data types subsystem. */
	void OpenDataTypeStatusWindow();
	void OnDataTypeStatusWindowClosed(const TSharedRef<SWindow>& ClosedWindow);

	/** Opens the plugin settings in the project settings window. */
	void OpenPluginSettings() const;

#pragma endregion Command Functions
};