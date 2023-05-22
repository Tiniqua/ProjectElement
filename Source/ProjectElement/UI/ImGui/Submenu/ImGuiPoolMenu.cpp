// Fill out your copyright notice in the Description page of Project Settings.

#include "ImGuiPoolMenu.h"
#include "Components/World/ActorPool.h"

bool UImGuiPoolMenu::Initialise(AController* InOwningController)
{
	if (!Super::Initialise(InOwningController))
	{
		return false;
	}

	// Extra steps here.
	return true;
}

void UImGuiPoolMenu::ShowSubmenu()
{
	if (!OwningController)
	{
		ImGui::Text("No owning controller?");
		return;
	}
	
	const UWorld* CurrentWorld = OwningController->GetWorld();
	if (!CurrentWorld)
	{
		return;
	}

	TArray<UActorPool*> Pools = TArray<UActorPool*>();
	for (TObjectIterator<UActorPool> poolIt; poolIt; ++poolIt)
	{
		if (poolIt->GetWorld() != CurrentWorld)
		{
			continue;
		}

		Pools.Add(*poolIt);
	}

	if (Pools.Num() <= 0)
	{
		ImGui::Text("No active pools found...");
		return;
	}

	auto ShowPoolItemRow([](UActorPool* Pool, AActor* PooledActor, bool bActive)
	{
		const auto TextColor = bActive ? IM_COL32(0, 255, 0, 100) : IM_COL32(255, 0, 0, 100);
		ImGui::PushStyleColor(ImGuiCol_Text, TextColor);

		const FString PoolActorName = PooledActor->GetName();
		ImGui::Text(TCHAR_TO_ANSI(*PoolActorName));
		ImGui::NextColumn();

		ImGui::Text(TCHAR_TO_ANSI(*PooledActor->GetActorLocation().ToString()));
		ImGui::NextColumn();

		ImGui::Text(TCHAR_TO_ANSI(*PooledActor->GetActorRotation().ToString()));
		ImGui::NextColumn();
		ImGui::PopStyleColor();

		if (bActive)
		{
			const char* ButtonLabel = TCHAR_TO_ANSI(*("Remove##" + PoolActorName));
			if (ImGui::Button(ButtonLabel))
			{
				Pool->DespawnActorInPool(PooledActor);
			}
		}
		ImGui::NextColumn();
	});
	
	for (UActorPool* Pool : Pools)
	{
		if (ImGui::TreeNode(TCHAR_TO_ANSI(*Pool->GetPoolName().ToString())))
		{
			ImGui::Columns(4);
			
			ImGui::Text("Actor Name");
			ImGui::NextColumn();

			ImGui::Text("Location");
			ImGui::NextColumn();

			ImGui::Text("Rotation");
			ImGui::NextColumn();
			ImGui::NextColumn();
			
			for (AActor* InWorld : Pool->GetActorsInWorld())
			{
				ShowPoolItemRow(Pool, InWorld, true);
			}
			
			for (AActor* InPool : Pool->GetActorsInPool())
			{
				ShowPoolItemRow(Pool, InPool, false);
			}

			ImGui::Columns(1);
			ImGui::TreePop();
		}

		ImGui::Separator();
	}
}