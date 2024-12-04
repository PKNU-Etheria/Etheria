// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/GA/EPGA_EnemyGetHit.h"
#include "Character/NPC/Enemy/Monster/Monster_Base.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UEPGA_EnemyGetHit::UEPGA_EnemyGetHit()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEPGA_EnemyGetHit::ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMonster_Base* Monster = CastChecked<AMonster_Base>(ActorInfo->AvatarActor.Get());
	Monster->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// TODO : Montage Null check

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MonsterGetHit"), Monster->GetGetHitMontage(), 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UEPGA_EnemyGetHit::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UEPGA_EnemyGetHit::OnInterruptedCallback);

	PlayAttackTask->ReadyForActivation();
}

void UEPGA_EnemyGetHit::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AMonster_Base* Monster = CastChecked<AMonster_Base>(ActorInfo->AvatarActor.Get());
	Monster->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UEPGA_EnemyGetHit::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UEPGA_EnemyGetHit::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
