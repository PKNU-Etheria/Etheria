// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h" // 애니메이션 노티파이에 태그를 사용하기 위해 헤더파일 추가
#include "AnimNotify_EPAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UAnimNotify_EPAttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_EPAttackHitCheck();

protected:
	// 애니메이션 노티파이에 이름을 지정해주는 함수
	virtual FString GetNotifyName_Implementation() const override;
	//
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	// Notify에 사용할 태그
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	float ComboAttackLevel;
};
