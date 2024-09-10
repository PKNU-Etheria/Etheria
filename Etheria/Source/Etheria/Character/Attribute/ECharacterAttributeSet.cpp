// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Attribute/ECharacterAttributeSet.h"

UECharacterAttributeSet::UECharacterAttributeSet() : 
	AttackRate(100.f),
	MaxAttackRate(300.f),
	MaxHealth(500.f),
	MoveSpeed(50.f),
	MaxMoveSpeed(300.f)
{
	InitHealth(GetMaxHealth());
}

void UECharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UECharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
}
