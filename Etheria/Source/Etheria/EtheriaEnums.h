// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETestEnum : uint8
{
	ETE_Test,
};

// 아이템 등급
UENUM()
enum class EItemQuality : uint8
{
	Normal UMETA(DisplayName = "Normal"),	// 일반
	Rear UMETA(DisplayName = "Rear"),	// 레어
	Epic UMETA(DisplayName = "Epic"),	// 희귀
	Legend UMETA(DisplayName = "Legend"),	// 전설
	Special UMETA(DisplayName = "Special"),	// 특별
};

// 아이템 타입
UENUM()
enum class EItemType : uint8
{
	Ingredient UMETA(DisplayName = "Ingredient"), // 재료 아이템
	Weapon UMETA(DisplayName = "Weapon"),	// 무기
	Consumable UMETA(DisplayName = "Consumable"),	// 소비 아이템
	Quest UMETA(DisplayName = "Quest"),	// 퀘스트 아이템
};