// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_JumpAndWaitForLanding.h"

#include "GameFramework/Character.h"

UAT_JumpAndWaitForLanding::UAT_JumpAndWaitForLanding()
{
}

UAT_JumpAndWaitForLanding* UAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	// 해당 어빌리티 태스크가 작업 완료를 기다리고 있으므로 가비지 컬렉터에 의해 수거되지 않도록 대기
	SetWaitingOnAvatar();
	// SetWaitingOnTargetData()
	// SetWaitingOnGameplayEvent()
}

void UAT_JumpAndWaitForLanding::OnDestroy(bool InOwnerFinished)
{
	// Ability에서 EndAbility 호출 뒤 태스크의 On Destroy 함수 호출
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UAT_JumpAndWaitForLanding::OnLandedCallback);
	
	Super::OnDestroy(InOwnerFinished);
}

void UAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& HitResult)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCompleted.Broadcast();
	}
}
