// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/GE/EPGE_AttackDamage.h"
#include "Character/ECharacterAttributeSet.h"

UEPGE_AttackDamage::UEPGE_AttackDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
}
