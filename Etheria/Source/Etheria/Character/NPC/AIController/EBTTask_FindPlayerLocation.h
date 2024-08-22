// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EBTTask_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UEBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UEBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExcuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Search, meta = (AllowProtectedAccess = true))
	bool bSearchRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Search, meta = (AllowProtectedAccess = true))
	float SearchRadius = 150.f;
};
