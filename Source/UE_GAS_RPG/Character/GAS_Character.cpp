// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_Character.h"

#include "UE_GAS_RPG/Player/GAS_PlayerState.h"

AGAS_Character::AGAS_Character()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* AGAS_Character::GetAbilitySystemComponent() const
{
	return ASC;
}


