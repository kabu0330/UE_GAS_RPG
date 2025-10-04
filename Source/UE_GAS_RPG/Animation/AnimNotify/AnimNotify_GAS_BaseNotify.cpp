// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_GAS_BaseNotify.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_GAS_BaseNotify::UAnimNotify_GAS_BaseNotify()
{

}

FString UAnimNotify_GAS_BaseNotify::GetNotifyName_Implementation() const
{
	return TEXT("GAS_BaseNotify");
}

void UAnimNotify_GAS_BaseNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AActor* OwnerActor = MeshComp->GetOwner(); IsValid(OwnerActor))
		{
			FGameplayEventData EventData;
			EventData.EventMagnitude = Level;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, EventData);
		}
	}
}

