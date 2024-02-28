
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHudWidget.generated.h"

class UProgressBar;
UCLASS()
class PROJECTELEMENT_API UPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealth(float CurrentHealth, float MaxHealth) const;
	
	void SetPower(float CurrentPower, float MaxPower) const;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* PowerBar = nullptr;
};
