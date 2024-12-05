// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Monster_Slime.h"
#include "Monster_AssetRef.h"

AMonster_Slime::AMonster_Slime()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(SLIME_DEAD_MOTION);
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(SLIME_ATTACK_MOTION);
	if (AttackActionMontageRef.Object)
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitMontageRef(SLIME_GETHIT_MOTION);
	if (GetHitMontageRef.Object)
	{
		GetHitMontage = GetHitMontageRef.Object;
	}
}
