// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GA/EPGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AT/EPAT_Trace.h"
#include "TA/EPTA_Trace.h"
#include "Character/ECharacterAttributeSet.h"

UEPGA_AttackHitCheck::UEPGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEPGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("UEPGA_AttackHitCheck : ActivateAbility"));

	CurrentLevel = TriggerEventData->EventMagnitude;

	UEPAT_Trace* AttackTraceTask = UEPAT_Trace::CreateTask(this, AEPTA_Trace::StaticClass());

	AttackTraceTask->OnComplete.AddDynamic(this, &UEPGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UEPGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		UE_LOG(LogTemp, Warning, TEXT("UEPGA_AttackHitCheck : Target %s Detected"), *(HitResult.GetActor()->GetName()));

		// GE ?¬ìš©
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UEPGA_AttackHitCheck : Target Not Detected"));
	}

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
