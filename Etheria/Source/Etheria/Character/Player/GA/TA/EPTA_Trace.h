// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "EPTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AEPTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AEPTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

protected:
	// 타겟팅이 완료가 되면 Confirm을 했을 때, Conform한 상태에서 물리 판정을 수행해주기 위해 Handle을 return 해야한다.
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bShowDebug = false;
	
};
