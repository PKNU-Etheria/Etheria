// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EtheriaStructs.generated.h"

USTRUCT(BlueprintType)
struct ETHERIA_API FQuestStruct : public FTableRowBase
{
	GENERATED_BODY()
	FQuestStruct() : QuestID(-1) {}

	FQuestStruct(const FQuestStruct& other)
	{
		QuestID = other.QuestID;
		QuestName = other.QuestName;
		QuestDescription = other.QuestDescription;

		Progress_Monster = other.Progress_Monster;
		Progress_Monster_RequireNum = other.Progress_Monster_RequireNum;
		Progress_Monster_ID = other.Progress_Monster_ID;

		Progress_Item = other.Progress_Item;
		Progress_Item_RequireNum = other.Progress_Item_RequireNum;
		Progress_Item_ID = other.Progress_Item_ID;
	}

	FQuestStruct& operator=(const FQuestStruct& other)
	{
		QuestID = other.QuestID;
		QuestName = other.QuestName;
		QuestDescription = other.QuestDescription;

		Progress_Monster = other.Progress_Monster;
		Progress_Monster_RequireNum = other.Progress_Monster_RequireNum;
		Progress_Monster_ID = other.Progress_Monster_ID;

		Progress_Item = other.Progress_Item;
		Progress_Item_RequireNum = other.Progress_Item_RequireNum;
		Progress_Item_ID = other.Progress_Item_ID;
		return *this;
	}

	// 퀘스트 정보

		// 퀘스트 ID
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int QuestID;

		// 퀘스트 이름
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName QuestName;

		// 퀘스트 설명
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText QuestDescription;



	// 퀘스트 전제 조건 (Require)
	
		// 선행 레벨
		int Require_Level = 1;

		// 선행 아이템
		TArray<int> Require_Items;

		// 선행 퀘스트
		TArray<int> Require_Quests;
	


	// 퀘스트 진행도 (Progress)

		// 몬스터 처치
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> Progress_Monster;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Monster_RequireNum;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Monster_ID;

		// 아이템 획득
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Item;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Item_RequireNum;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName>  Progress_Item_ID;



	// 퀘스트 보상

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

// 아이템 수치 정보
USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float ArmorRating;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float RestorationAmount;

	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Descriptioin;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

// 수치형 데이터
USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

// 아이콘, 메쉬 정보
USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics FItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
};