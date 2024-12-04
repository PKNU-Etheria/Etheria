// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GA/EPGA_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "Character/ECharacterAttributeSet.h"
#include "Character/NPC/Enemy/Monster/GA/EPGA_EnemyGetHit.h"

UEPGA_AttackHitCheck::UEPGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEPGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("UEPGA_AttackHitCheck : ActivateAbility"));

	CurrentLevel = TriggerEventData->EventMagnitude;

	FHitResult OutHitResult;

	if (TriggerEventData && TriggerEventData->TargetData.Num() > 0)
	{
		TSharedPtr<FGameplayAbilityTargetData> SharedTargetData = TriggerEventData->TargetData.Data[0];
		if (SharedTargetData.IsValid() && SharedTargetData->GetScriptStruct() == FGameplayAbilityTargetData_SingleTargetHit::StaticStruct())
		{
			const FGameplayAbilityTargetData_SingleTargetHit* SingleTargetData =
				static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(SharedTargetData.Get());

			if (SingleTargetData)
			{
				OutHitResult = SingleTargetData->HitResult;
			}
		}
	}

	FGameplayAbilityTargetDataHandle DataHandle;

	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
	DataHandle.Add(TargetData);

	OnTraceResultCallback(DataHandle);
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
