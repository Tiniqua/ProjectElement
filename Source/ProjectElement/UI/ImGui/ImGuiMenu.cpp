
#include "ImGuiMenu.h"
#include "ImGuiModule.h"
#include "Submenu/ImGuiDebugCommandsMenu.h"
#include "Submenu/ImGuiPoolMenu.h"
#include "Submenu/ImGuiSubmenu.h"

#if !UE_BUILD_SHIPPING
bool UImGuiMenu::Initialise(AController* InOwningController)
{
	OwningController = InOwningController;
	if (!OwningController)
	{
		UE_LOG(LogImGui, Warning, TEXT("%s: Cannot initialise Imgui menu because the owning controller is null!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	// Construct submenu's
	Submenus =
	{
		NewObject<UImGuiDebugCommandsMenu>(),
		NewObject<UImGuiPoolMenu>()
	};

	for (UImGuiSubmenu* Submenu : Submenus)
	{
		if (!Submenu)
		{
			continue;
		}

		Submenu->Initialise(OwningController);
	}
	
	return true;
}

void UImGuiMenu::SetIsEnabled(const bool bEnableImGui)
{
	if (bIsEnabled == bEnableImGui)
	{
		return;
	}

	bIsEnabled = bEnableImGui;
	bWindowOpen = bEnableImGui;

	if (FImGuiModule* ImGuiModule = FModuleManager::GetModulePtr<FImGuiModule>("ImGui"))
	{
		ImGuiModule->SetInputMode(bIsEnabled);
	}
}

void UImGuiMenu::Draw()
{
	if (!bIsEnabled)
	{
		return;
	}

	if (!bWindowOpen)
	{
		SetIsEnabled(false);
		return;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {400.0f,250.0f });
	ImGui::Begin("Project Element Debug Window", &bWindowOpen);

	if (Submenus.Num() <= 0)
	{
		ImGui::Text("No sub menu's exist to show? Check LogImGui logs for warnings/errors.");
		ImGui::End();
		return;
	}
	
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("DebugTabBar", tab_bar_flags))
	{
		for (UImGuiSubmenu* Submenu : Submenus)
		{
			if (!Submenu)
			{
				continue;
			}

			if (Submenu->IsInitialised())
			{
				Submenu->Draw();
			}
		}
			
		ImGui::EndTabBar();
	}
	ImGui::Separator();
	ImGui::End();
}
#endif
