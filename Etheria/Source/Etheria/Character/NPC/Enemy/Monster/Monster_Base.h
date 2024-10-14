// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NPC/Enemy/Enemy_Base.h"
#include "Monster_Base.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AMonster_Base : public AEnemy_Base
{
	GENERATED_BODY()
	
public:
	AMonster_Base();

	FORCEINLINE virtual class UAnimMontage* GetAttackActionMontage() const { return AttackActionMontage; }

protected:


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackActionMontage;

	float DeadEventDelayTime = 5.0f;
};
