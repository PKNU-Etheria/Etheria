// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayerState.h"
#include "AbilitySystemComponent.h"

AEPlayerState::AEPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));	
}

UAbilitySystemComponent* AEPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
