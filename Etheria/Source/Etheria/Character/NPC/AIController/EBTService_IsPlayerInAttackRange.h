// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "EBTService_IsPlayerInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UEBTService_IsPlayerInAttackRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UEBTService_IsPlayerInAttackRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 150.f;
};
