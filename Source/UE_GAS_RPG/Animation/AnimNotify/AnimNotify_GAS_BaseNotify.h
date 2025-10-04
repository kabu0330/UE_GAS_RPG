// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_GAS_BaseNotify.generated.h"

/**
 * 
 */
UCLASS()
class UE_GAS_RPG_API UAnimNotify_GAS_BaseNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_GAS_BaseNotify();
	
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, meta = (Categories = Event))
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	float Level = 1.0f;
};
