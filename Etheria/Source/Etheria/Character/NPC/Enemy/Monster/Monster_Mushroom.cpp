// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Monster_Mushroom.h"
#include "Monster_AssetRef.h"

AMonster_Mushroom::AMonster_Mushroom()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(MUSHROOM_DEAD_MOTION);
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(MUSHROOM_ATTACK_MOTION);
	if (AttackActionMontageRef.Object)
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitMontageRef(MUSHROOM_GETHIT_MOTION);
	if (GetHitMontageRef.Object)
	{
		GetHitMontage = GetHitMontageRef.Object;
	}
}
