
#include "Components/Abilities/BaseTelegraphedAbility.h"

#include "Actors/Character/ElementCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"


UBaseTelegraphedAbility::UBaseTelegraphedAbility()
{
	// TODO - Permentantly ticks atm because its active on player abilitycomponent its just not been activated
	PrimaryComponentTick.bCanEverTick = true;
}


void UBaseTelegraphedAbility::StartAbility()
{
	Super::StartAbility();

	AElementCharacter* OwningCharacter = Cast<AElementCharacter>(OwnerCharacter);
	AElementPlayerControllerBase* PlayerController = OwningCharacter->GetPlayerController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
	}

	IsTelegraphPendingConfirm = true;

	NiagaraComponent = NewObject<UNiagaraComponent>(this);
	NiagaraComponent->SetAsset(TelegraphVFX);
	NiagaraComponent->SetupAttachment(OwnerCharacter->GetRootComponent());
	NiagaraComponent->bAutoActivate = false;
	NiagaraComponent->RegisterComponent();
}

void UBaseTelegraphedAbility::CreateTelegraph(float DeltaTime)
{
	// Get the player controller and check if it's valid
	AElementCharacter* OwningCharacter = Cast<AElementCharacter>(OwnerCharacter);
	const AElementPlayerControllerBase* PlayerController = OwningCharacter->GetPlayerController();
	if (!PlayerController)
	{
		return;
	}

	// Get the world location and direction of the mouse cursor
	FVector WorldLocation, WorldDirection;
	if (!PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		return;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);
	// Trace from the cursor location down to the ground to find the location to spawn the debug sphere
	FHitResult HitResult;
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), WorldLocation, WorldLocation - WorldDirection * -3000.f, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	if (!bHit)
	{
		return;
	}

	CurrentAbilityLocation = HitResult.ImpactPoint;

	// TODO - Move to debug menu as option behind CVAR
	if (ShouldDrawDebug)
	{
		DrawDebugSphere(GetWorld(),
			HitResult.ImpactPoint,
			50.f,
			12, FColor::Red,
			false,
			DeltaTime);
	}

	if (NiagaraComponent && TelegraphVFX)
	{
		NiagaraComponent->SetWorldLocation(CurrentAbilityLocation);
		NiagaraComponent->ActivateSystem();
	}
}

void UBaseTelegraphedAbility::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsTelegraphPendingConfirm)
	{
		CreateTelegraph(DeltaTime);
	}
}

void UBaseTelegraphedAbility::ActivateTelegraphedAbility()
{
	Super::ActivateTelegraphedAbility();
}

void UBaseTelegraphedAbility::EndAbility()
{
	Super::EndAbility();

	// Deactivate and destroy the Niagara system component
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
		NiagaraComponent->DestroyComponent();
		NiagaraComponent = nullptr;
	}
	
	// Unbind OnClicked event from the player controller
	AElementCharacter* OwningCharacter = Cast<AElementCharacter>(OwnerCharacter);
	AElementPlayerControllerBase* PlayerController = OwningCharacter->GetPlayerController();

	// Reset input mode to game only
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}

	IsTelegraphPendingConfirm = false;
}
