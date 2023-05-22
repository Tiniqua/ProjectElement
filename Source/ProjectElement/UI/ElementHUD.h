// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Globals.h"
#include "GameFramework/HUD.h"
#include "ElementHUD.generated.h"

class UImGuiMenu;

UCLASS()
class PROJECTELEMENT_API AElementHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(Transient)
	UImGuiMenu* ImGuiMenu = nullptr;

public:

	virtual void PostInitializeComponents() override;
	
	/** Add extra functionality on-top of this to allow the ImGui module to be updated. */
	virtual void DrawHUD() override;

	/* Show the debug menu drawn using ImGui. */
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowDebugMenu(int32 bEnabled);
};
