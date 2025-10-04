// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_CharacterBase.h"
#include "AbilitySystemInterface.h"
#include "GAS_Character.generated.h"

class UGAS_ComboActionData;
class UAttributeSet;

UCLASS()
class UE_GAS_RPG_API AGAS_Character : public AGAS_CharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGAS_Character();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	virtual void InitAbilitySystemActorInfo() {}
	
	UGAS_ComboActionData* GetComboActionData() { return ComboActionData; }

protected:
	UPROPERTY(EditAnywhere, Category = "GAS|Defalut")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Data")
	TObjectPtr<UGAS_ComboActionData> ComboActionData;
	
public:

};
