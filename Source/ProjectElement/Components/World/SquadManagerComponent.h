#pragma once

#include "CoreMinimal.h"
#include "Actors/Character/ElementCharacterBase.h"
#include "Components/ActorComponent.h"
#include "SquadManagerComponent.generated.h"

class AElementCharacterBase;  // Forward declaration

USTRUCT(BlueprintType)
struct FSquadInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AElementCharacterBase*> Members;
	
	UPROPERTY()
	AElementCharacterBase* SquadLeader;

	UPROPERTY(BlueprintReadWrite, Category = "Squad")
	int32 CurrentSquadSize;
	
	UPROPERTY(BlueprintReadWrite, Category = "Squad")
	int32 MaxSquadSize;

	UPROPERTY()
	bool IsEngaged = false;
	
	UPROPERTY()
	bool CanAddMembers = true;

	FSquadInfo()
	{
		SquadLeader = nullptr;
		CurrentSquadSize = 0;
		MaxSquadSize = 5;
		IsEngaged = false;
		CanAddMembers = true;
	}

	// custom comparison for structs
	bool operator==(const FSquadInfo& Other) const
	{
		// Implement the logic for comparing struct members
		return Members == Other.Members &&
			   SquadLeader == Other.SquadLeader &&
			   CurrentSquadSize == Other.CurrentSquadSize &&
			   MaxSquadSize == Other.MaxSquadSize &&
			   IsEngaged == Other.IsEngaged &&
			   CanAddMembers == Other.CanAddMembers;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTELEMENT_API USquadManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USquadManagerComponent();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Squads")
	TArray<FSquadInfo> Squads;

public:
	
	void CreateSquad(AElementCharacterBase* Creator);
	void AddMemberToSquad(AElementCharacterBase* Member, FSquadInfo& Squad) const;
	void RemoveMemberFromSquad(AElementCharacterBase* Member, FSquadInfo& Squad) const;
	void UpdateSquadLeadership(FSquadInfo& Squad) const;
	AElementCharacterBase* FindHighestRankedMember(const FSquadInfo& Squad) const;

};
