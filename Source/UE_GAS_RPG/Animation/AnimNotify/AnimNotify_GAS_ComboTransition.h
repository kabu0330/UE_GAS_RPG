// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_GAS_RPG/Animation/AnimNotify/AnimNotify_GAS_BaseNotify.h"
#include "AnimNotify_GAS_ComboTransition.generated.h"

/**
 * 
 */
UCLASS()
class UE_GAS_RPG_API UAnimNotify_GAS_ComboTransition : public UAnimNotify_GAS_BaseNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_GAS_ComboTransition();

	virtual FString GetNotifyName_Implementation() const override;
protected:
	
};
