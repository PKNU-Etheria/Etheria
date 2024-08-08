// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Gameframework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "EBTTask_RandomMove.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ETHERIA_API UEBTTask_RandomMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UEBTTask_RandomMove();

	EBTNodeResult::Type ExcuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Search, meta = (AllowPrivateAccess = true))
	float SearchRadius = 1500.f;
};
