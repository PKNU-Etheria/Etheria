// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/AIController/EBTService_ChangeSpeed.h"
#include "Character/NPC/Enemy/Enemy_Base.h"
#include "EAIController.h"
#include "Character/ECharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UEBTService_ChangeSpeed::UEBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UEBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	Super::OnBecomeRelevant(ownerComp, nodeMemory);

	auto const Controller = ownerComp.GetAIOwner();
	AECharacter* const Enemy = Cast<AECharacter>(Controller->GetPawn());

	Enemy->GetCharacterMovement()->MaxWalkSpeed = speed;
}

FString UEBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the Enemy Speed");
}
