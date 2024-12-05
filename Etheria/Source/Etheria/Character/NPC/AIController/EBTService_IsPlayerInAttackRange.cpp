// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/AIController/EBTService_IsPlayerInAttackRange.h"
#include "Character/NPC/Enemy/Enemy_Base.h"
#include "EAIController.h"
#include "Character/ECharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UEBTService_IsPlayerInAttackRange::UEBTService_IsPlayerInAttackRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Attack Range");
}

void UEBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::OnBecomeRelevant(ownerComp, nodeMemory);

	AEAIController* const Controller = Cast<AEAIController>(ownerComp.GetAIOwner());
	AECharacter* const Enemy = Cast<AECharacter>(Controller->GetPawn());

	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	Controller->GetBlackboard()->SetValueAsBool(BlackboardKeys::PlayerIsInAttackRange, Enemy->GetDistanceTo(Player) <= AttackRange);
}
