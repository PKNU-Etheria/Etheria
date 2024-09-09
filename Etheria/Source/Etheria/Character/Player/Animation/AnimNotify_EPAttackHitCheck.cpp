// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Animation/AnimNotify_EPAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotify_EPAttackHitCheck::UAnimNotify_EPAttackHitCheck()
{
	ComboAttackLevel = 1.0f;
}

FString UAnimNotify_EPAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("EPAttackHitCheck");
}

void UAnimNotify_EPAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = ComboAttackLevel;
			// 내가 지정한 ASC를 가진 특정 액터에 태그를 넣어서 이벤트를 발동시키는 함수
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}
