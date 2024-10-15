// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GA/EPGA_Dash.h"
#include "Character/Player/EPlayer.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Player/Tag/EPlayerGameAbilityTag.h"

UEPGA_Dash::UEPGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEPGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AEPlayer* EPlayer = CastChecked<AEPlayer>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), EPlayer->GetDashMontage());

	PlayAttackTask->OnCompleted.AddDynamic(this, &UEPGA_Dash::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UEPGA_Dash::OnInterruptedCallback);

	FVector LaunchVelocity = EPlayer->GetActorForwardVector() * 1000;
	

	// ¶¥ÀÌ¶û ÇÏ´Ã ±¸ºÐ
	if (!EPlayer->GetAbilitySystemComponent()->HasMatchingGameplayTag(PLAYERTAG_STATE_ISINAIR))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Is Not Jumping!"));
		LaunchVelocity *= 5;
	}
	
	LaunchVelocity.Z = 0;
	EPlayer->LaunchCharacter(LaunchVelocity, false, false);

	PlayAttackTask->ReadyForActivation();
}

void UEPGA_Dash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UEPGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AEPlayer* EPlayer = CastChecked<AEPlayer>(ActorInfo->AvatarActor.Get());
}

void UEPGA_Dash::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UEPGA_Dash::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
