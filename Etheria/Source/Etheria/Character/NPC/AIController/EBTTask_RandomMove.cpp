// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/AIController/EBTTask_RandomMove.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "NavigationSystem.h"
#include "Etheria/Character/NPC/AIController/EAIController.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Etheria/Character/NPC/AIController/BlackboardKeys.h"

UEBTTask_RandomMove::UEBTTask_RandomMove()
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UEBTTask_RandomMove::ExcuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const cont = Cast<AEAIController>(OwnerComp.GetAIOwner());
	auto const npc = cont->GetPawn();

	FVector const origin = npc->GetActorLocation();
	FNavLocation loc;

	UNavigationSystemV1* const navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (navSystem->GetRandomPointInNavigableRadius(origin, SearchRadius, loc, nullptr))
	{
		cont->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, loc.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
