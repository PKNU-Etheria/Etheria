// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Monster_Cactus.h"
#include "Monster_AssetRef.h"

AMonster_Cactus::AMonster_Cactus()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(CACTUS_DEAD_MOTION);
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(CACTUS_ATTACK_MOTION);
	if (AttackActionMontageRef.Object)
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitMontageRef(CACTUS_GETHIT_MOTION);
	if (GetHitMontageRef.Object)
	{
		GetHitMontage = GetHitMontageRef.Object;
	}
}
