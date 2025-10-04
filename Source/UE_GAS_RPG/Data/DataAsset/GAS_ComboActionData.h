// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GAS_ComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class UE_GAS_RPG_API UGAS_ComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UGAS_ComboActionData();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere)
	FString SectionName = TEXT("ComboAttack");
		
	UPROPERTY(EditAnywhere)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere)
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere)
	float FrameRate;
};
