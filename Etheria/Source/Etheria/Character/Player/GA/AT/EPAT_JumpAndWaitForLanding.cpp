// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GA/AT/EPAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UEPAT_JumpAndWaitForLanding::UEPAT_JumpAndWaitForLanding()
{
}

UEPAT_JumpAndWaitForLanding* UEPAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UEPAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UEPAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UEPAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());

	Character->LandedDelegate.AddDynamic(this, &UEPAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UEPAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UEPAT_JumpAndWaitForLanding::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UEPAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
