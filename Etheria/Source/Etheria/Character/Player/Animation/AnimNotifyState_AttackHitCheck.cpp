// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/Animation/AnimNotifyState_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"

UAnimNotifyState_AttackHitCheck::UAnimNotifyState_AttackHitCheck()
{
	DamageRate = 0.0f;
}

FString UAnimNotifyState_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("EPAttackHitCheck");
}

void UAnimNotifyState_AttackHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UAnimNotifyState_AttackHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	// Use GameplayEffect

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			FGameplayEventData PayloadData;
			PayloadData.EventMagnitude = DamageRate;
			// 내가 지정한 ASC를 가진 특정 액터에 태그를 넣어서 이벤트를 발동시키는 함수
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}

void UAnimNotifyState_AttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
