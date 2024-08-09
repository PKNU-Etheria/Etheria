// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// ����Ʈ ��ȣ�ۿ� �������
UENUM(BlueprintType)
enum class EQuestInteractStatus : uint8
{
	EQIS_None,
	EQIS_Accepting,
	EQIS_Clearing,
};

// ������ ���
UENUM()
enum class EItemQuality : uint8
{
	Normal UMETA(DisplayName = "Normal"),	// �Ϲ�
	Rear UMETA(DisplayName = "Rear"),	// ����
	Epic UMETA(DisplayName = "Epic"),	// ���
	Legend UMETA(DisplayName = "Legend"),	// ����
	Special UMETA(DisplayName = "Special"),	// Ư��
};

// ������ Ÿ��
UENUM()
enum class EItemType : uint8
{
	Ingredient UMETA(DisplayName = "Ingredient"), // ��� ������
	Weapon UMETA(DisplayName = "Weapon"),	// ����
	Consumable UMETA(DisplayName = "Consumable"),	// �Һ� ������
	Quest UMETA(DisplayName = "Quest"),	// ����Ʈ ������
};