
#include "UI/PlayerHudWidget.h"

#include "Components/ProgressBar.h"

void UPlayerHudWidget::SetHealth(const float CurrentHealth, const float MaxHealth) const
{
	if(HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UPlayerHudWidget::SetPower(const float CurrentPower, const float MaxPower) const
{
	if(PowerBar)
	{
		PowerBar->SetPercent(CurrentPower / MaxPower);
	}
}
