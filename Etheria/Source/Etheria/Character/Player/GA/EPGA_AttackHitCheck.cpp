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

		// GE 사용
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			// Source로부터 Target(HitResult)에게 GE를 발동시키라는 함수
			// 인자 (GA의 핸들, 액터 정보, 어빌리티 발동 정보, 발동시킬 GE 정보, 콜백 함수 타겟 핸들 정보)
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
