// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterAttributeSet.h"

UECharacterAttributeSet::UECharacterAttributeSet() :
	Health(100.0f),
	MaxHealth(1000.0f),
	GroundSpeed(1000.0f),
	MaxGroundSpeed(2000.0f),
	AttackPow(50.0f),
	MaxAttackPow(1000.0f),
	Defense(50.0f),
	MaxDefense(1000.0f),
	AttackSpeed(1.0f),
	MaxAttackSpeed(4.0f)
{
	InitHealth(GetMaxHealth());
}

void UECharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

void UECharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{

}
