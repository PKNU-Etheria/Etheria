// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/AIController/EBTTask_BasicAttack.h"
#include "Character/NPC/AIController/EAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/NPC/AIController/ECombatInterface.h"
#include "Character/NPC/Enemy/Monster/GA/EPGA_MonsterMeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackboardKeys.h"
#include "AIController.h"

UEBTTask_BasicAttack::UEBTTask_BasicAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Enemy BasicAttack");
}

EBTNodeResult::Type UEBTTask_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEAIController* const Controller = Cast<AEAIController>(OwnerComp.GetAIOwner());
	AEnemy_Base* const Enemy = Cast<AEnemy_Base>(Controller->GetPawn());

	if (Controller->GetBlackboard()->GetValueAsBool(BlackboardKeys::PlayerIsInAttackRange)) 
	{
		auto const ASC = Enemy->GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayAbilitySpec* BasicAttackSpec = ASC->FindAbilitySpecFromClass(UEPGA_MonsterMeleeAttack::StaticClass());

			if (BasicAttackSpec)
			{
				ASC->TryActivateAbility(BasicAttackSpec->Handle);
			}
		}
		/*if (IECombatInterface* const iCombat = Cast<IECombatInterface>(Enemy))
		{
			if (MontageHasFinished(Enemy))
			{
				iCombat->Execute_BasicAttack(Enemy);
			}
		}*/
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Type();
}

bool UEBTTask_BasicAttack::MontageHasFinished(AEnemy_Base* const Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetAttackActionMontage());
}
