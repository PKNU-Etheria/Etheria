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
