// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EtheriaEnums.h"
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

	// ����Ʈ ����

		// ����Ʈ ID
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int QuestID;

		// ����Ʈ �̸�
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName QuestName;

		// ����Ʈ ����
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText QuestDescription;



	// ����Ʈ ���� ���� (Require)
	
		// ���� ����
		int Require_Level = 1;

		// ���� ������
		TArray<int> Require_Items;

		// ���� ����Ʈ
		TArray<int> Require_Quests;
	


	// ����Ʈ ���൵ (Progress)

		// ���� óġ
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> Progress_Monster;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Monster_RequireNum;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Monster_ID;

		// ������ ȹ��
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Item;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>  Progress_Item_RequireNum;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName>  Progress_Item_ID;



	// ����Ʈ ����

};

// ������ ��ġ ����
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

// ��ġ�� ������
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
// ������, �޽� ����
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
	FName ID;	// �⺻ ������ ���̺� �˻� �ʵ� ����

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