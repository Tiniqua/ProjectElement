#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/World/SquadManagerComponent.h"

#include "SquadManager.generated.h"

UCLASS()
class PROJECTELEMENT_API ASquadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASquadManager();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USquadManagerComponent* SquadManagerComponent;
	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
