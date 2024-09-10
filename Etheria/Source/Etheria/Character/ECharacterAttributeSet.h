// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ECharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ETHERIA_API UECharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UECharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, GroundSpeed);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxGroundSpeed);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, AttackPow);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxAttackPow);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, Defense);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxDefense);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, AttackSpeed);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxAttackSpeed);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxGroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPow;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackPow;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxDefense;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackSpeed;

	// State
	
};
