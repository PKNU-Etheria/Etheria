// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ECharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Character/Player/Tag/EPlayerGameAbilityTag.h"


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
	MaxAttackSpeed(4.0f),
	Damage(0.0f)
{
	InitHealth(GetMaxHealth());
}

void UECharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//if (Attribute == GetHealthAttribute())
	//{
	//	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	//}

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UECharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(PLAYERTAG_STATE_INVINSIBLE))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;
}

void UECharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	// Effect를 통해서 변동된 정보가 Data로 들어옴, Modifier 정보들이 들어옴
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Direct Health Access : %f"), GetHealth());
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), GetDamage());
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		// 대상(ASC)에다가 메뉴얼로 태그를 지정(특정한 어빌리티나 이펙트 X)
		// 따라서 죽으면 해당 ASC에 해당 태그가 부착이 된다
		Data.Target.AddLooseGameplayTag(PLAYERTAG_STATE_ISDEAD);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}

//void UECharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
//{
//	if (Attribute == GetHealthAttribute())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Health: %f -> %f"), OldValue, NewValue);
//	}
//}
