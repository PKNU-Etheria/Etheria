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
	FQuestStruct() {}

	// ����Ʈ ����

		// NPC ID
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NPCID;

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
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Require_Level = 1;

		// ���� ������
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> Require_Items;

		// ���� ����Ʈ
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
		TArray<int>  Progress_Item_ID;



	// ����Ʈ ����

};

USTRUCT(BlueprintType)
struct ETHERIA_API FBranchStruct : public FTableRowBase
{
	GENERATED_BODY()
	FBranchStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NextScriptID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Branch_Script;
};

USTRUCT(BlueprintType)
struct ETHERIA_API FDialogueStruct : public FTableRowBase
{
	GENERATED_BODY()
	FDialogueStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SceneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ScriptID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Script;


	// ���� ��ũ��Ʈ ID (Branch O)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBranchStruct> Branchs;

	// ���� ��ũ��Ʈ ID (Branch X)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NextScriptID;


	// ���� Scene ID�� ScriptID���� �ൿ�� NPC ID (Behavior Tree�� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> ActionNPC_IDs;


	// Should Loop
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoop = false;
};

// ����Ʈ ��ȭ ���� ���� 
USTRUCT(BlueprintType)
struct ETHERIA_API FQuestDialogueDataStruct : public FTableRowBase
{
	GENERATED_BODY()
	FQuestDialogueDataStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int QuestID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AcceptDialogueDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ClearDialogueDB;
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
	UStaticMesh* Mesh;	// No Animation

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;	// if Item Have Animation
};


USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	FItemData() {};

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

USTRUCT()
struct FWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	FWeaponData() {};

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Weapon Data")
	FItemAssetData AssetData;
};