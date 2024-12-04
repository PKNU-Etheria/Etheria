// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/GA/EPGA_EnemyAttackHitCheck.h"
#include "Character/Player/GA/AT/EPAT_Trace.h"
#include "Character/Player/GA/TA/EPTA_Trace.h"

#include "AbilitySystemBlueprintLibrary.h"

UEPGA_EnemyAttackHitCheck::UEPGA_EnemyAttackHitCheck()
{
}

void UEPGA_EnemyAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Log, TEXT("UEPGA_EnemyAttackHitCheck : ActivateAbility"));

	CurrentLevel = TriggerEventData->EventMagnitude;
	
	UEPAT_Trace* AttackTraceTask = UEPAT_Trace::CreateTask(this, AEPTA_Trace::StaticClass());

	AttackTraceTask->OnComplete.AddDynamic(this, &UEPGA_EnemyAttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UEPGA_EnemyAttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
