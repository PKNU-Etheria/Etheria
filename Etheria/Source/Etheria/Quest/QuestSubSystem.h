// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "EtheriaStructs.h"
#include "QuestSubSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FQuestAvailable_Delegate, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FQuestClearable_Delegate, int32)

UCLASS()
class ETHERIA_API UQuestSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UQuestSubSystem();

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void InitializeNPC(class ANPC_Base* NPC);
	const TArray<FQuestStruct*>& GetAllQuests() { return AllQuests; }
	
public:
	// Related to Inventory (Quantity = Player Owning Item Qunatity)
	// Unavailable && Progressing Check
	void UpdateQuests_Item(int itemID, int Quantity);

	// Related to Monster
	// Progressing Check
	void UpdateQuests_Monster(int MonsterID);

	// Update From NPC (Player Interacts With NPC)
	// Available -> Progressing
	void AcceptQuest(int QuestID);

	// Update From NPC (Player Interacts With NPC)
	// Unavailable Check
	// Clearable -> Clear
	void ClearQuest(int QuestID);

private:
	// Check Quest Available
	bool CheckQuestAvailable(int QuestID);

	// Check Quest Cleared
	bool CheckQuestClearable(int QuestID);
	

public:
	FQuestAvailable_Delegate Available_Delegate;
	FQuestClearable_Delegate Clearable_Delegate;

private:
	UPROPERTY()
	UDataTable* QuestDB;

	TArray<FQuestStruct*> AllQuests;

	TMap<int, FQuestStruct*> UnavailableQuests;
	TMap<int, FQuestStruct*> AvailableQuests;
	TMap<int, FQuestStruct*> ProgressingQuests;
	TMap<int, FQuestStruct*> ClearableQuests;
	TMap<int, FQuestStruct*> ClearedQuests;
};
