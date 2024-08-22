// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/AIController/EBTTask_FindPlayerLocation.h"
#include "EBTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Etheria/Character/NPC/AIController/EAIController.h"
#include "Etheria/Character/NPC/AIController/BlackboardKeys.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


UEBTTask_FindPlayerLocation::UEBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UEBTTask_FindPlayerLocation::ExcuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Controller = Cast<AEAIController>(OwnerComp.GetAIOwner());

	const FVector PlayerLocation = Player->GetActorLocation();

	if (bSearchRandom) 
	{
		FNavLocation TargetLocation;

		const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, TargetLocation, nullptr))
		{
			Controller->GetBlackboard()->SetValueAsVector(BlackboardKeys::TargetLocation, TargetLocation.Location);
		}
	}
	else 
	{
		Controller->GetBlackboard()->SetValueAsVector(BlackboardKeys::TargetLocation, PlayerLocation);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Type(EBTNodeResult::Succeeded);
}
