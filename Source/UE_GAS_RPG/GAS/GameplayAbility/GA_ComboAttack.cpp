// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_ComboAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UE_GAS_RPG/Character/GAS_Character.h"
#include "UE_GAS_RPG/Data/DataAsset/GAS_ComboActionData.h"
#include "UE_GAS_RPG/Data/Tag/GAS_GameplayTags.h"

UGA_ComboAttack::UGA_ComboAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AnimNotifyComboEventTag = TAG_EVENT_CHARACTER_ACTION_ISATTACKING;
}

bool UGA_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	bHasNextComboInput = false;
	CurrentComboCount = 0;
	
	AGAS_Character* Character = CastChecked<AGAS_Character>(ActorInfo->AvatarActor.Get());
	ComboActionData = Character->GetComboActionData();

	// 캐릭터 스텟 제한
	MaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed * 0.6f;
	
	if (!ComboActionData)
	{
		UE_LOG(LogTemp, Error, TEXT("GA_Attack : 캐릭터에 콤보 액션 데이터 에셋이 null입니다."));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
	
	MontageAndWaitTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
this, TEXT("ComboAttack"), ComboActionData->ComboActionMontage, 1.0f, GetNextSection());

	if (MontageAndWaitTask)
	{
		MontageAndWaitTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleteCallback);
		MontageAndWaitTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
		Character->PlayAnimMontage(ComboActionData->ComboActionMontage);
		MontageAndWaitTask->ReadyForActivation();
	}

	UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, AnimNotifyComboEventTag);
	if (GameplayEventTask)
	{
		GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnComboEventReceived);
		GameplayEventTask->ReadyForActivation();
	}
}

void UGA_ComboAttack::OnComboEventReceived(FGameplayEventData Payload)
{
	if (Payload.EventTag == AnimNotifyComboEventTag)
	{
		if (bHasNextComboInput)
		{
			bHasNextComboInput = false;

			MontageJumpToSection(GetNextSection());
		}
	}
}

FName UGA_ComboAttack::GetNextSection()
{
	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, ComboActionData->MaxComboCount);
	const FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->SectionName, CurrentComboCount);
	return NextSection;
}

void UGA_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	// 동작 실행 이후 또 입력이 들어오면
	if (CurrentComboCount > 0)
	{
		bHasNextComboInput = true;
	}
}

void UGA_ComboAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_ComboAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_ComboAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	AGAS_Character* Character = CastChecked<AGAS_Character>(ActorInfo->AvatarActor.Get());
	Character->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	//Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UGA_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	AGAS_Character* Character = CastChecked<AGAS_Character>(ActorInfo->AvatarActor.Get());
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Character->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	ComboActionData = nullptr;
	CurrentComboCount = 0;
	bHasNextComboInput = false;
}



