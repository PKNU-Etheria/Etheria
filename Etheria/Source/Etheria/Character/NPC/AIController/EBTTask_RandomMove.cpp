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

UEBTTask_RandomMove::UEBTTask_RandomMove(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UEBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Controller = Cast<AEAIController>(OwnerComp.GetAIOwner());
	auto const NPC = Controller->GetPawn();

	FVector const NPCLocation = NPC->GetActorLocation();
	FNavLocation TargetLocation;

	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (NavSystem->GetRandomPointInNavigableRadius(NPCLocation, SearchRadius, TargetLocation, nullptr))
	{
		Controller->GetBlackboardComponent()->SetValueAsVector(BlackboardKeys::TargetLocation, TargetLocation.Location);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
