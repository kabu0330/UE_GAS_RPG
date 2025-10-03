// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAS_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_GAS_RPG_API AGAS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAS_GameModeBase();
	virtual void BeginPlay() override;
};
