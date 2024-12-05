// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Character/ECharacterAttributeSet.h"

AEPlayerState::AEPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));	
	AttributeSet = CreateDefaultSubobject<UECharacterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AEPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
