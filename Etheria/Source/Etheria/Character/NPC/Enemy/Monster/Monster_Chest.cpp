// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Monster_Chest.h"
#include "Monster_AssetRef.h"

AMonster_Chest::AMonster_Chest()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(CHEST_DEAD_MOTION);
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(CHEST_ATTACK_MOTION);
	if (AttackActionMontageRef.Object)
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitMontageRef(CHEST_GETHIT_MOTION);
	if (GetHitMontageRef.Object)
	{
		GetHitMontage = GetHitMontageRef.Object;
	}
}
