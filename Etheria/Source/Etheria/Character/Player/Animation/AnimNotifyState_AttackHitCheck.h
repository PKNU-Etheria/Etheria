// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UAnimNotifyState_AttackHitCheck : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_AttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category = Attack)
	FName StartSocket;

	UPROPERTY(EditAnywhere, Category = Attack)
	FName EndSocket;

	UPROPERTY(EditAnywhere, Category = Attack)
	float Radius = 0;

	UPROPERTY(EditAnywhere)
	float Damage = 0;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

	TArray<AActor*> DamagedActors;

private:
	class USkeletalMeshComponent* MeshComponent;
	bool TimeDilationApplied = false;
	FTimerHandle TimeDilationHandle;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	float CurrentLevel;
};
