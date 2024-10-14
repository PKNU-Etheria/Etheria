// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Monster_Beholder.h"
#include "Animation/AnimMontage.h"
#include "Monster_AssetRef.h"

AMonster_Beholder::AMonster_Beholder()
{

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(BEHOLDER_ATTACK_MOTION);
	if (AttackActionMontageRef.Object)
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(BEHOLDER_DEAD_MOTION);
	if (DeadMontageRef.Object) 
	{
		DeadMontage = DeadMontageRef.Object;
	}
}
