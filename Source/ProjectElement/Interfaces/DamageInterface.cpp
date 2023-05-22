// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageInterface.h"
#include "Damage/BaseDamageType.h"

bool IDamageInterface::Damage_Implementation(const UBaseDamageType* DamageType, AActor* InDamageSource)
{
	return false;
}
