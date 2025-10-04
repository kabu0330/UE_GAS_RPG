// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_Enemy.h"

#include "AbilitySystemComponent.h"
#include "UE_GAS_RPG/GAS/Attribute/GAS_AttributeSetBase.h"


AGAS_Enemy::AGAS_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetNetUpdateFrequency(100.0f);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSetBase>("AttributeSet");
}


void AGAS_Enemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilitySystemActorInfo();
}

void AGAS_Enemy::InitAbilitySystemActorInfo()
{
	Super::InitAbilitySystemActorInfo();

	ASC->InitAbilityActorInfo(this, this);
}



