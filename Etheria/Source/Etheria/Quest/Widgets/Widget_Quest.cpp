// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Widgets/Widget_Quest.h"
#include "Quest/QuestSubSystem.h"
#include "Quest/Widgets/Widget_QuestList.h"
#include "Components/ListView.h"
#include "EtheriaStructs.h"
#include "Kismet/GameplayStatics.h"

void UWidget_Quest::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(this);
	if (!GameInst) return;

	QuestList_Startable->SetQuestWidget(this);
	QuestList_Progressing->SetQuestWidget(this);
	QuestList_Cleared->SetQuestWidget(this);

	UQuestSubSystem* QuestSystem = GameInst->GetSubsystem<UQuestSubSystem>();
	if (QuestSystem)
	{
		const TMap<int, FQuestStruct*>& UnavailableQuests = QuestSystem->GetUnavailableQuests();
		for (const auto& quest : UnavailableQuests)
			QuestList_Startable->AddUnAvailableQuest(quest.Value);

		const TMap<int, FQuestStruct*>& AvailableQuests = QuestSystem->GetAvailableQuests();
		for (const auto& quest : AvailableQuests)
			QuestList_Startable->AddAvailableQuest(quest.Value);

		const TMap<int, FQuestStruct*>& ProgressingQuests = QuestSystem->GetProgressingQuests();
		for (const auto& quest : ProgressingQuests)
			QuestList_Progressing->AddProgressingQuest(quest.Value); 
		
		const TMap<int, FQuestStruct*>& ClearableQuests = QuestSystem->GetClearableQuests();
		for (const auto& quest : ClearableQuests)
			QuestList_Progressing->AddClearableQuest(quest.Value);

		const TMap<int, FQuestStruct*>& ClearedQuest = QuestSystem->GetClearedQuests ();
		for (const auto& quest : ClearedQuest)
			QuestList_Cleared->AddClearedQuest(quest.Value);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UWidget_Quest : Failed to get QuestSubSystem."));
	}
}

void UWidget_Quest::ShowQuests()
{
	
}
