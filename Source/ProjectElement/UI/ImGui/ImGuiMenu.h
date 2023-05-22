// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Globals.h"
#include "UObject/Object.h"
#include "ImGuiMenu.generated.h"

class AController;
class UImGuiSubmenu;

UCLASS()
class PROJECTELEMENT_API UImGuiMenu : public UObject
{
	GENERATED_BODY()

#if !UE_BUILD_SHIPPING
public:

	bool Initialise(AController* InOwningController);
	void SetIsEnabled(const bool bEnableImGui);
	bool IsEnabled() const { return bIsEnabled; }
	
	void Draw();
#endif

protected:

	UPROPERTY(Transient)
	AController* OwningController = nullptr;

	UPROPERTY(Transient)
	TArray<UImGuiSubmenu*> Submenus;
	
private:

	bool bIsEnabled = false;
	bool bWindowOpen = true;
};
