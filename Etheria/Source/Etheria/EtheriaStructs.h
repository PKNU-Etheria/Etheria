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

	// 퀘스트 정보

		// NPC ID
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int NPCID;

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
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Require_Level = 1;

		// 선행 아이템
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> Require_Items;

		// 선행 퀘스트
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
		TArray<int>  Progress_Item_ID;



	// 퀘스트 보상

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


	// 다음 스크립트 ID (Branch O)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBranchStruct> Branchs;

	// 다음 스크립트 ID (Branch X)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NextScriptID;


	// 현재 Scene ID의 ScriptID에서 행동할 NPC ID (Behavior Tree로 제어)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> ActionNPC_IDs;


	// Should Loop
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoop = false;
};

// 퀘스트 대화 정보 저장 
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
	FName ID;	// 기본 데이터 테이블 검색 필드 역할

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