// Fill out your copyright notice in the Description page of Project Settings.

#include "ImGuiSubmenu.h"

#if !UE_BUILD_SHIPPING

bool UImGuiSubmenu::Initialise(AController* InOwningController)
{
	if (!InOwningController)
	{
		UE_LOG(LogImGui, Warning, TEXT("%s: Cannot initialise submenu because the owning controller is null!"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	OwningController = InOwningController;
	bInitialised = true;
	return true;
}

void UImGuiSubmenu::Draw()
{
	if (ImGui::BeginTabItem(TCHAR_TO_ANSI(*SubmenuTitle)))
	{
		ShowSubmenu();
		ImGui::EndTabItem();
	}
}	

void UImGuiSubmenu::ShowSubmenu()
{
	// Implement in subclass.
}
#endif