// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Globals.h"
#include "UObject/Object.h"
#include "ImGuiSubmenu.generated.h"

class AController;

UCLASS(Abstract)
class PROJECTELEMENT_API UImGuiSubmenu : public UObject
{
	GENERATED_BODY()

#if !UE_BUILD_SHIPPING

public:

	/** Initialise the owning controller for child classes usage.
	 *  NOTE: Override this to init any other debug tracking classes for a given sub-menu. */
	virtual bool Initialise(AController* InOwningController);

	/** Can this sub menu be rendered or did it fail initialisation? */
	bool IsInitialised() const { return bInitialised; }

	/** Draw the ImGui code inside this submenu. (ShowSubmenu) */
	void Draw();

protected:

	/** Override this and add any ImGui code here in a child class. */
	virtual void ShowSubmenu();

#endif
	
	UPROPERTY(Transient)
	AController* OwningController = nullptr;

	FString SubmenuTitle;

	bool bInitialised = false;
};
