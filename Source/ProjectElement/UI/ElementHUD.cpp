// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementHUD.h"
#include "Actors/Controllers/ElementPlayerControllerBase.h"
#include "ImGui/ImGuiMenu.h"

DEFINE_LOG_CATEGORY_STATIC(LogElementHUD, Warning, All);

void AElementHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!PlayerOwner)
	{
		UE_LOG(LogElementHUD, Warning, TEXT("%s: Player owner is null?! Cannot initialise HUD: %s"), ANSI_TO_TCHAR(__FUNCTION__), *GetName());
		return;
	}

#if !UE_BUILD_SHIPPING
	ImGuiMenu = NewObject<UImGuiMenu>();
	ImGuiMenu->Initialise(PlayerOwner);
#endif
}

void AElementHUD::DrawHUD()
{
	Super::DrawHUD();

// If ImGui is enabled render and return, do not render other UI in the canvas.
#if !UE_BUILD_SHIPPING
	if (ImGuiMenu && ImGuiMenu->IsEnabled())
	{
		ImGuiMenu->Draw();
		return;
	}
#endif
}

void AElementHUD::ShowDebugMenu(int32 bEnabled)
{
#if !UE_BUILD_SHIPPING
	if (!ImGuiMenu)
	{
		if (PlayerOwner)
		{
			UE_LOG(LogElementHUD, Error, TEXT("%s: Imgui menu refference is null in the HUD owned by %s"), ANSI_TO_TCHAR(__FUNCTION__), *PlayerOwner->GetName());
		}
		
		return;
	}

	ImGuiMenu->SetIsEnabled((bool)bEnabled);
#endif
}