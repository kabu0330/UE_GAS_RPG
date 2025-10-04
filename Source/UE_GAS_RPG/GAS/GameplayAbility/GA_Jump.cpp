// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Jump.h"

#include "GameFramework/Character.h"
#include "UE_GAS_RPG/GAS/AbilityTask/AT_JumpAndWaitForLanding.h"

UGA_Jump::UGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (!bResult)
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (IsValid(Character) && Character->CanJump());
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 점프를 어빌리티 태스크에서 수행하도록 변경
	// ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	// Character->Jump();

	UAT_JumpAndWaitForLanding* JumpAndWaitForLandingTask = UAT_JumpAndWaitForLanding::CreateTask(this);
	JumpAndWaitForLandingTask->OnCompleted.AddDynamic(this, &UGA_Jump::OnLandedCallback);
	JumpAndWaitForLandingTask->ReadyForActivation();
}

void UGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

void UGA_Jump::OnLandedCallback()
{
	// EndAbility가 호출되기 전까지 태그가 활성화되어 있다.
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
