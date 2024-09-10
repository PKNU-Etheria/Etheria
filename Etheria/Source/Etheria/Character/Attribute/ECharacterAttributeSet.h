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
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxMoveSpeed);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UECharacterAttributeSet, MaxAttackRate);

	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Move", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Move", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

};
