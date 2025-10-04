// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "UE_GAS_RPG/GAS/Attribute/GAS_AttributeSetBase.h"


AGAS_PlayerState::AGAS_PlayerState()
{
	SetReplicates(true);
	SetNetUpdateFrequency(100.0f);
	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSetBase>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AGAS_PlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

UAttributeSet* AGAS_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

