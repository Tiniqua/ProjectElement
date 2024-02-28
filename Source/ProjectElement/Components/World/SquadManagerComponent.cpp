#include "Components/World/SquadManagerComponent.h"
#include "Actors/Character/ElementAiCharacter.h"

USquadManagerComponent::USquadManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USquadManagerComponent::CreateSquad(AElementCharacterBase* Creator)
{
	FSquadInfo NewSquad;
	
	Squads.Add(NewSquad);

	AddMemberToSquad(Creator, NewSquad);
}

void USquadManagerComponent::AddMemberToSquad(AElementCharacterBase* Member, FSquadInfo& Squad) const
{
	if (Squad.CanAddMembers && Squad.CurrentSquadSize < Squad.MaxSquadSize)
	{
		Squad.Members.Add(Member);
		Squad.CurrentSquadSize++;

		if (AElementAiCharacter* AiMember = Cast<AElementAiCharacter>(Member))
		{
			AiMember->SetCurrentSquad(&Squad);
		}
		
		UpdateSquadLeadership(Squad);
		return;
	}
	// ---- SQUAD FULL -----
	if(Squad.CurrentSquadSize == Squad.MaxSquadSize)
	{
		Squad.CanAddMembers = false;
	}
}

void USquadManagerComponent::RemoveMemberFromSquad(AElementCharacterBase* Member, FSquadInfo& Squad) const
{
	for (int32 MemberIndex = 0; MemberIndex < Squad.Members.Num(); ++MemberIndex)
	{
		if (Squad.Members[MemberIndex] == Member)
		{
			Squad.Members.RemoveAt(MemberIndex);
			Squad.CurrentSquadSize--;

			if (AElementAiCharacter* AiMember = Cast<AElementAiCharacter>(Member))
			{
				AiMember->SetCurrentSquad(nullptr);
			}

			if (Squad.CurrentSquadSize < Squad.MaxSquadSize)
			{
				Squad.CanAddMembers = true;
			}

			UpdateSquadLeadership(Squad);
			return;
		}
	}
	// if we get here we are trying to remove a member from the wrong squad
}


void USquadManagerComponent::UpdateSquadLeadership(FSquadInfo& Squad) const
{
	if(Squad.CurrentSquadSize == 1 && Squad.CurrentSquadSize != 0)
	{
		Squad.SquadLeader = Squad.Members[0];
	}
	// SQUAD LEADER ASSIGNMENT SYSTEM
	else
	{
		AElementCharacterBase* HighestRankedMember = FindHighestRankedMember(Squad);

		if (HighestRankedMember != nullptr)
		{
			Squad.SquadLeader = HighestRankedMember;
		}
	}
}

AElementCharacterBase* USquadManagerComponent::FindHighestRankedMember(const FSquadInfo& Squad) const
{
	AElementCharacterBase* HighestRankedMember = nullptr;
	ERank HighestRank = ERank::Unassigned;

	for (AElementCharacterBase* Member : Squad.Members)
	{
		// Ensure the member is of type AElementAiCharacter
		if (AElementAiCharacter* AiMember = Cast<AElementAiCharacter>(Member))
		{
			// Compare ranks
			if (AiMember->Rank > HighestRank)
			{
				HighestRank = AiMember->Rank;
				HighestRankedMember = AiMember;
			}
		}
	}

	return HighestRankedMember;
}

