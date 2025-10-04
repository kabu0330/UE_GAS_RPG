// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_Character.h"
#include "GAS_Enemy.generated.h"

UCLASS()
class UE_GAS_RPG_API AGAS_Enemy : public AGAS_Character
{
	GENERATED_BODY()

public:
	AGAS_Enemy();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilitySystemActorInfo() override;

	
};
