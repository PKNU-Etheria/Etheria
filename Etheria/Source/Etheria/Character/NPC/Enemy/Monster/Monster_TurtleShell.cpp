// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/Monster/Monster_TurtleShell.h"
#include "Monster_AssetRef.h"

AMonster_TurtleShell::AMonster_TurtleShell()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TURTLESHELL_DEAD_MOTION);
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackActionMontageRef(TURTLESHELL_ATTACK_MOTION);
	if (AttackActionMontageRef.Object)
	{
		AttackActionMontage = AttackActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GetHitMontageRef(TURTLESHELL_GETHIT_MOTION);
	if (GetHitMontageRef.Object)
	{
		GetHitMontage = GetHitMontageRef.Object;
	}
}
