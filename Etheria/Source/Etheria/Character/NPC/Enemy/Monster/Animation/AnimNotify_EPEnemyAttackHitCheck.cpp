// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Animation/AnimNotify_EPEnemyAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_EPEnemyAttackHitCheck::UAnimNotify_EPEnemyAttackHitCheck()
{
}

FString UAnimNotify_EPEnemyAttackHitCheck::GetNotifyName_Implementation() const
{
	
	return TEXT("GasEnemyAttackHitCheck");
}

void UAnimNotify_EPEnemyAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();

		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
