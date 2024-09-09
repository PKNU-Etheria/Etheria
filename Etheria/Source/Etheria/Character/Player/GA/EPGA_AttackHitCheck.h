// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EPGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UEPGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEPGA_AttackHitCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	// GA에서 UE를 사용하기 위함 (블루프린트에서 지정)
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	float CurrentLevel;
};
