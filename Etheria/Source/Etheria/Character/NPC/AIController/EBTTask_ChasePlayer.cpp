// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/AIController/EBTTask_ChasePlayer.h"
#include "Etheria/Character/NPC/AIController/EAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Etheria/Character/NPC/AIController/BlackboardKeys.h"

UEBTTask_ChasePlayer::UEBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UEBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEAIController* const Controller = Cast<AEAIController>(OwnerComp.GetAIOwner());
	FVector const PlayerLocation = Controller->GetBlackboard()->GetValueAsVector(BlackboardKeys::TargetLocation);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
