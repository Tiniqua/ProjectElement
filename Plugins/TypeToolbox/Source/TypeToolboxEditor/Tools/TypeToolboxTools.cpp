// Fill out your copyright notice in the Description page of Project Settings.

#include "TypeToolboxTools.h"
#include "ISettingsModule.h"
#include "LevelEditor.h"
#include "Interfaces/IMainFrameModule.h"
#include "TypeToolbox/DataType/DataTypeSubsystem.h"
#include "Widgets/Layout/SScrollBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogToolboxTools, Log, All);

#define LOCTEXT_NAMESPACE "ToolboxTools"

/** Commands class for registering commands to editor. */
class TToolboxToolsCommands : public TCommands<TToolboxToolsCommands>
{
public:

	TToolboxToolsCommands() : TCommands<TToolboxToolsCommands>(
		TEXT("ToolboxTools"),
		FText::FromString("Tool commands for the Type Toolbox plugin."),
		NAME_None,
		FAppStyle::GetAppStyleSetName()) {}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(RefreshDataTypesCommand, "Refresh Data Types", "Refresh the data types and tables in the Data Type Subsystem.", EUserInterfaceActionType::Button, FInputGesture());
		UI_COMMAND(OpenDataTypesStatusWindowCommand, "Data Types Status", "Opens the data types status window.", EUserInterfaceActionType::Button, FInputGesture());
		UI_COMMAND(OpenPluginSettings, "Plugin Settings", "Opens the plugin settings in the project settings window.", EUserInterfaceActionType::Button, FInputGesture());
	}
	
	TSharedPtr<FUICommandInfo> RefreshDataTypesCommand;
	TSharedPtr<FUICommandInfo> OpenDataTypesStatusWindowCommand;
	TSharedPtr<FUICommandInfo> OpenPluginSettings;
};

void FTypeToolboxTools::StartupTools()
{
	CommandList = MakeShareable(new FUICommandList);
	TToolboxToolsCommands::Register();

	// Map commands to functions.
	const auto& Commands = TToolboxToolsCommands::Get();
	
	CommandList->MapAction(
		Commands.RefreshDataTypesCommand,
		FExecuteAction::CreateRaw(this, &FTypeToolboxTools::RefreshDataTypes),
		FCanExecuteAction());

	CommandList->MapAction(
		Commands.OpenDataTypesStatusWindowCommand,
		FExecuteAction::CreateRaw(this, &FTypeToolboxTools::OpenDataTypeStatusWindow),
		FCanExecuteAction());

	CommandList->MapAction(
	Commands.OpenPluginSettings,
	FExecuteAction::CreateRaw(this, &FTypeToolboxTools::OpenPluginSettings),
	FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorMenuExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();

	// Create tools menu extender under the level editor toolbar.
	TypeToolsMenuExtender = MakeShareable(new FExtender);
	TypeToolsMenuExtender->AddMenuBarExtension("Window", EExtensionHook::After, CommandList, FMenuBarExtensionDelegate::CreateRaw(this, &FTypeToolboxTools::MakeTypeToolboxMenu));
	LevelEditorMenuExtensibilityManager->AddExtender(TypeToolsMenuExtender);
}

void FTypeToolboxTools::ShutdownTools()
{
	TToolboxToolsCommands::Unregister();
}

void FTypeToolboxTools::MakeTypeToolboxMenu(FMenuBarBuilder& InMenuBuilder)
{
	InMenuBuilder.AddPullDownMenu(
		FText::FromString("Type Toolbox"),
		FText::FromString("Settings and commands for interacting with the TypeToolbox plugin."),
		FNewMenuDelegate::CreateRaw(this, &FTypeToolboxTools::PopulateTypeToolboxMenu),
		"TypeToolbox",
		FName(TEXT("Type Toolbox"))
	);
}

void FTypeToolboxTools::PopulateTypeToolboxMenu(FMenuBuilder& InMenuBuilder) const
{
	InMenuBuilder.BeginSection("Commands", FText::FromString("Commands"));
	InMenuBuilder.AddMenuEntry(TToolboxToolsCommands::Get().RefreshDataTypesCommand);
	InMenuBuilder.EndSection();

	InMenuBuilder.BeginSection("Settings", FText::FromString("Settings"));
	InMenuBuilder.AddMenuEntry(TToolboxToolsCommands::Get().OpenDataTypesStatusWindowCommand);
	InMenuBuilder.AddMenuEntry(TToolboxToolsCommands::Get().OpenPluginSettings);
	InMenuBuilder.EndSection();
}

#pragma region Command Functions

void FTypeToolboxTools::RefreshDataTypes() const
{
	UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogToolboxTools, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	DataTypeSubsystem->MarkSubsystemDirty();
}

void FTypeToolboxTools::OpenDataTypeStatusWindow()
{
	if (DataTypeStatusWindow.IsValid())
	{
		return;
	}

	UDataTypeSubsystem* DataTypeSubsystem = GEngine->GetEngineSubsystem<UDataTypeSubsystem>();
	if (!DataTypeSubsystem)
	{
		UE_LOG(LogToolboxTools, Error, TEXT("%s: Cannot find the UDataTypeSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	// Create list.
	const TSharedRef<SVerticalBox> DataTypeStatusList = SNew(SVerticalBox);
	for (const TPair<FString, bool>& DataTypeLoadStatePair : DataTypeSubsystem->DataTypeLoadState)
	{
		const FText TypeLoadStateText = FText::AsCultureInvariant(DataTypeLoadStatePair.Key);
		const FLinearColor StateColor = DataTypeLoadStatePair.Value ? FLinearColor::Green : FLinearColor::Red;
		
		DataTypeStatusList->AddSlot()
			.HAlign(HAlign_Center)
			.Padding(FMargin(0.0f, 15.0f, 0.0f, 0.0f))
			[
				SNew(STextBlock)
				.Text(TypeLoadStateText)
				.ColorAndOpacity(StateColor)
			];
	}

	// Show list in window.
	DataTypeStatusWindow = SNew(SWindow)
	.Title(FText::FromString(TEXT("Data Types Status")))
	.ClientSize(FVector2D(300, 800))
	.SupportsMaximize(false)
	.SupportsMinimize(false)
	[
		SNew(SScrollBox)
		.AllowOverscroll(EAllowOverscroll::No)
		+ SScrollBox::Slot()
		[
			DataTypeStatusList
		]
	];

	// Add window to main frame.
	const IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(DataTypeStatusWindow.ToSharedRef(), MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(DataTypeStatusWindow.ToSharedRef());
	}

	DataTypeStatusWindow->SetOnWindowClosed(FOnWindowClosed::CreateRaw(this, &FTypeToolboxTools::OnDataTypeStatusWindowClosed));
}

void FTypeToolboxTools::OnDataTypeStatusWindowClosed(const TSharedRef<SWindow>& ClosedWindow)
{
	DataTypeStatusWindow = nullptr;
}

void FTypeToolboxTools::OpenPluginSettings() const
{
	FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(FName("Project"), FName("Game"), FName("TypeToolboxSettings"));
}

#pragma endregion Command Functions

#undef LOCTEXT_NAMESPACE