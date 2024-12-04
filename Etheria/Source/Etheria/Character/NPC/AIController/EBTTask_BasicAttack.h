// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Character/NPC/Enemy/Enemy_Base.h"
#include "EBTTask_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UEBTTask_BasicAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UEBTTask_BasicAttack(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool MontageHasFinished(AEnemy_Base* const Enemy);
};
