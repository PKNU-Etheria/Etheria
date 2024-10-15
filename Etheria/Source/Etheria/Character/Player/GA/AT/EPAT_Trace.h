// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "EPAT_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ETHERIA_API UEPAT_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UEPAT_Trace();

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UEPAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AEPTA_Trace> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinializeTargetActor();

protected:
	//TA의 델리게이트 구독을 위한 함수
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);
	
public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AEPTA_Trace> TargetActorClass;

	TObjectPtr<class AEPTA_Trace> SpawnedTargetActor;
};
