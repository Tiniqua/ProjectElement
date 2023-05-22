// Fill out your copyright notice in the Description page of Project Settings.

#include "ImGuiDebugCommandsMenu.h"

#include "Actors/Controllers/ElementPlayerControllerBase.h"

bool UImGuiDebugCommandsMenu::Initialise(AController* InOwningController)
{
	if (!Super::Initialise(InOwningController))
	{
		return false;
	}

	// Extra steps here.
	return true;
}

void UImGuiDebugCommandsMenu::ShowSubmenu()
{
	if (!OwningController)
	{
		ImGui::Text("No owning controller?");
		return;
	}

	AElementPlayerControllerBase* RSPlayerController = Cast<AElementPlayerControllerBase>(OwningController);
	if (!RSPlayerController)
	{
		ImGui::Text("No RSPlayerController controller, this is required to use the debug commands.");
		return;
	}

	if (ImGui::TreeNode("Character"))
	{
		ImGui::Columns(2);

		ImGui::Text("God Mode");
		ImGui::NextColumn();
		
		const char* GodModeText = RSPlayerController->bGodModeEnabled ? "Disable" : "Enable";
		if (ImGui::Button(GodModeText))
		{
			RSPlayerController->bGodModeEnabled = !RSPlayerController->bGodModeEnabled;
		}
		ImGui::NextColumn();

		ImGui::Text("Kill All");
		ImGui::NextColumn();
		
		if (ImGui::Button("Kill All"))
		{
			RSPlayerController->KillAll();
		}
		ImGui::NextColumn();

		// ETC.
		
		ImGui::Columns(1);
		ImGui::TreePop();
	}

	//ImGui::Separator();
}
