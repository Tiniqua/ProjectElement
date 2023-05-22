// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ImGuiSubmenu.h"
#include "ImGuiDebugCommandsMenu.generated.h"

UCLASS()
class PROJECTELEMENT_API UImGuiDebugCommandsMenu : public UImGuiSubmenu
{
	GENERATED_BODY()

public:
	
	UImGuiDebugCommandsMenu() { SubmenuTitle = "Debug Commands"; }

#if !UE_BUILD_SHIPPING
	
	virtual bool Initialise(AController* InOwningController) override;

protected:

	virtual void ShowSubmenu() override;

#endif
};
