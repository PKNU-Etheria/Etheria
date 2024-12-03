// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/GA/EPGA_MonsterMeleeAttack.h"
#include "Character/NPC/Enemy/Monster/Monster_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UEPGA_MonsterMeleeAttack::UEPGA_MonsterMeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEPGA_MonsterMeleeAttack::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMonster_Base* Monster = CastChecked<AMonster_Base>(ActorInfo->AvatarActor.Get());

	// TODO : Montage Null check

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MonsterAttack"), Monster->GetAttackActionMontage(), 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UEPGA_MonsterMeleeAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UEPGA_MonsterMeleeAttack::OnInterruptedCallback);

	PlayAttackTask->ReadyForActivation();
}

void UEPGA_MonsterMeleeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UEPGA_MonsterMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UEPGA_MonsterMeleeAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UEPGA_MonsterMeleeAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
