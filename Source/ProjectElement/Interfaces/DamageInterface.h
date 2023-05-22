// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

class UBaseDamageType;
UINTERFACE()
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTELEMENT_API IDamageInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	bool Damage(const UBaseDamageType* DamageType, AActor* InDamageSource);

	/** Returns false if the damage event depleted all health or vitality. */
	virtual bool Damage_Implementation(const UBaseDamageType* DamageType, AActor* InDamageSource);
};
