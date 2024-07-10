// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestSubSystem.h"
#include "QuestSubSystem.h"
#include "Character/NPC/NeutralNPC/NPC_Base.h"

UQuestSubSystem::UQuestSubSystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> QuestDBRef(TEXT("/Game/Character/NPC/NeutralNPC/Quest/Data/DB_Quest.DB_Quest"));
	if (QuestDBRef.Succeeded())
	{
		QuestDB = QuestDBRef.Object;

		QuestDB->GetAllRows<FQuestStruct>("UQuestSubSystem : GetAllRows", AllQuests);

		for (const auto& Quest : AllQuests)
		{
			// 선행 퀘스트 X && 요구사항 X && Level 1 
			if (Quest->Require_Quests.Num() == 0 && Quest->Require_Items.Num() == 0 && Quest->Require_Level <= 1)
				AvailableQuests.Add(TTuple<int, FQuestStruct*>(Quest->QuestID, Quest));
			else
				UnavailableQuests.Add(TTuple<int, FQuestStruct*>(Quest->QuestID, Quest));
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> QuestDialogueDBRef(TEXT("/Game/Character/NPC/NeutralNPC/Quest/Data/DB_QuestDialogues.DB_QuestDialogues"));
	if (QuestDialogueDBRef.Succeeded())
	{
		QuestDialogueDB = QuestDialogueDBRef.Object;

		TArray<FQuestDialogueDataStruct*> AllQuestDialogues;
		QuestDialogueDB->GetAllRows<FQuestDialogueDataStruct>("UQuestSubSystem : GetAllRows", AllQuestDialogues);

		for (const auto& Dialgoue : AllQuestDialogues)
			QuestDialogueMap.Add(TTuple<int, FQuestDialogueDataStruct*>(Dialgoue->QuestID, Dialgoue));
	}
}

void UQuestSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void UQuestSubSystem::InitializeNPC(ANPC_Base* NPC)
{
	for (auto& Quest : UnavailableQuests)
		if (Quest.Value->NPCID == NPC->NPCID)
			NPC->UnavailableQuests.Add(Quest);

	for (auto& Quest : AvailableQuests)
		if (Quest.Value->NPCID == NPC->NPCID)
			NPC->AvailableQuests.Add(Quest);

	for (auto& Quest : ProgressingQuests)
		if (Quest.Value->NPCID == NPC->NPCID)
			NPC->ProgressingQuests.Add(Quest);

	for (auto& Quest : ClearableQuests)
		if (Quest.Value->NPCID == NPC->NPCID)
			NPC->ClearableQuests.Add(Quest);

	for (auto& Quest : ClearedQuests)
		if (Quest.Value->NPCID == NPC->NPCID)
			NPC->ClearedQuests.Add(Quest);
}

void UQuestSubSystem::UpdateQuests_Item(int ItemID, int Quantity)
{
	// Unavailable Quests Update
	/*for (auto& Quest : UnavailableQuests)
	{
		for (int i = 0; i < Quest.Value->Require_Items.Num(); i++)
		{
			if ()
			{
				CheckQuestCleared(Quest.Key);
			}
		}
	}*/

	// Progressing Quests Update
	for (auto& Quest : ProgressingQuests)
	{
		for (int i = 0; i < Quest.Value->Progress_Item_ID.Num(); i++)
		{
			if (ItemID == Quest.Value->Progress_Item_ID[i] 
				&& Quest.Value->Progress_Item.IsValidIndex(i) && Quest.Value->Progress_Item_RequireNum.IsValidIndex(i)
				&& Quest.Value->Progress_Item[i] < Quest.Value->Progress_Item_RequireNum[i])
			{
				Quest.Value->Progress_Item[i] = Quantity;

				// NPC & Player Quest Update
				if (Quest.Value->Progress_Item[i] >= Quest.Value->Progress_Item_RequireNum[i])
				{
					Quest.Value->Progress_Item[i] = Quest.Value->Progress_Item_RequireNum[i];
					
					if (CheckQuestClearable(Quest.Key))
					{
						// Broadcast NPCs that this Quest is clearable.
						Clearable_Delegate.Broadcast(Quest.Value->NPCID, Quest.Key);
						
						// Unavailable -> Available
						ClearableQuests.Add(TTuple<int, FQuestStruct*>(Quest.Key, Quest.Value));
						ProgressingQuests.Remove(Quest.Key);
					}
				}
			}
		}
	}
}

void UQuestSubSystem::UpdateQuests_Monster(int MonsterID)
{
	// Progressing Quests Update
	for (auto& Quest : ProgressingQuests)
	{
		for (int i = 0; i < Quest.Value->Progress_Monster_ID.Num(); i++)
		{
			if (MonsterID == Quest.Value->Progress_Monster_ID[i]
				&& Quest.Value->Progress_Monster.IsValidIndex(i) && Quest.Value->Progress_Monster_RequireNum.IsValidIndex(i)
				&& Quest.Value->Progress_Monster[i] < Quest.Value->Progress_Monster_RequireNum[i])
			{
				Quest.Value->Progress_Monster[i] += 1;

				// NPC & Player Quest Update
				if (Quest.Value->Progress_Monster[i] >= Quest.Value->Progress_Monster_RequireNum[i])
				{
					Quest.Value->Progress_Monster[i] = Quest.Value->Progress_Monster_RequireNum[i];
					
					if (CheckQuestClearable(Quest.Key))
					{
						// Broadcast NPCs that this Quest is clearable.
						Clearable_Delegate.Broadcast(Quest.Value->NPCID, Quest.Key);

						// Unavailable -> Available
						ClearableQuests.Add(TTuple<int, FQuestStruct*>(Quest.Key, Quest.Value));
						ProgressingQuests.Remove(Quest.Key);
					}
				}
			}
		}
	}
}

void UQuestSubSystem::AcceptQuest(int QuestID)
{
	// Available -> Progressing
	FQuestStruct** QuestRefPtr = AvailableQuests.Find(QuestID);
	if (!QuestRefPtr) return;

	FQuestStruct* Quest = *(QuestRefPtr);
	if (!Quest) return;

	AvailableQuests.Remove(QuestID);
	ProgressingQuests.Add(TTuple<int, FQuestStruct*>(QuestID, Quest));

	// Check Quest is Clearable
	if (CheckQuestClearable(QuestID))
	{
		// Broadcast NPCs that this Quest is clearable.
		Clearable_Delegate.Broadcast(Quest->NPCID, QuestID);

		// Unavailable -> Available
		ClearableQuests.Add(TTuple<int, FQuestStruct*>(QuestID, Quest));
		ProgressingQuests.Remove(QuestID);
	}
}

void UQuestSubSystem::ClearQuest(int QuestID)
{
	// Clearable -> Cleared
	FQuestStruct** QuestRefPtr = ClearableQuests.Find(QuestID);
	if (!QuestRefPtr) return;

	FQuestStruct* QuestPtr = *(QuestRefPtr);
	if (!QuestPtr) return;

	ClearableQuests.Remove(QuestID);
	ClearedQuests.Add(TTuple<int, FQuestStruct*>(QuestID, QuestPtr));


	// Unavailable Quests Update
	for (auto& Quest : UnavailableQuests)
	{
		for (int i = 0; i < Quest.Value->Require_Quests.Num(); i++)
		{
			if (Quest.Value->Require_Quests[i] == QuestID)
			{
				if (CheckQuestAvailable(QuestID))
				{
					// Broadcast NPCs that this Quest is available.
					Available_Delegate.Broadcast(Quest.Value->NPCID, QuestID);

					// Unavailable -> Available
					AvailableQuests.Add(TTuple<int, FQuestStruct*>(Quest.Key, Quest.Value));
					UnavailableQuests.Remove(Quest.Key);
				}
			}
		}
	}
}

bool UQuestSubSystem::CheckQuestAvailable(int QuestID) 
{
	FQuestStruct** QuestRefPtr = UnavailableQuests.Find(QuestID);
	if (!QuestRefPtr)
		return false;

	FQuestStruct* Quest = *(QuestRefPtr);
	if (!Quest)
		return false;

	// Check Require Level is achieved


	// Check Require Items are all Owned
	for (int i = 0; i < Quest->Require_Items.Num(); i++)
	{
		
	}
	
	// Check Require Quests are all Cleared
	for (int i = 0; i < Quest->Require_Quests.Num(); i++)
	{
		if (!ClearedQuests.Contains(Quest->Require_Quests[i]))
			return false;
	}

	return true;
}

bool UQuestSubSystem::CheckQuestClearable(int QuestID)
{
	FQuestStruct** QuestRefPtr = ProgressingQuests.Find(QuestID);
	if (!QuestRefPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestSubSystem : Failed to get Quest (CheckQuestClearable)"));
		return false;
	}

	FQuestStruct* Quest = *(QuestRefPtr);
	if (!Quest) 
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestSubSystem : Failed to get Quest (CheckQuestClearable)"));
		return false;
	}

	for (int i = 0; i < Quest->Progress_Item.Num(); i++)
	{
		if (Quest->Progress_Item_RequireNum.IsValidIndex(i) &&
			Quest->Progress_Item[i] < Quest->Progress_Item_RequireNum[i])
			return false;
	}

	for (int i = 0; i < Quest->Progress_Monster.Num(); i++)
	{
		if (Quest->Progress_Monster_RequireNum.IsValidIndex(i) &&
			Quest->Progress_Monster[i] < Quest->Progress_Monster_RequireNum[i])
			return false;
	}

	return true;
}

const FQuestDialogueDataStruct* UQuestSubSystem::GetQuestDialgoue(int QuestID) 
{
	FQuestDialogueDataStruct** DialogueRef = QuestDialogueMap.Find(QuestID);
	if (!DialogueRef) return nullptr;

	return (*DialogueRef);
}
