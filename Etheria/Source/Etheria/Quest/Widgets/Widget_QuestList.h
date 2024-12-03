// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EtheriaStructs.h"
#include "Widget_QuestList.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UWidget_QuestList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetQuestWidget(class UWidget_Quest* parent);

public:
	void AddUnAvailableQuest(const FQuestStruct* InQuestInfo);
	void AddAvailableQuest(const FQuestStruct* InQuestInfo);
	void AddProgressingQuest(const FQuestStruct* InQuestInfo);
	void AddClearableQuest(const FQuestStruct* InQuestInfo);
	void AddClearedQuest(const FQuestStruct* InQuestInfo);

public:
	void QuestIsAvailable(int32 NPCID, int32 QuestID);
	void QuestIsClearable(int32 NPCID, int32 QuestID);
	void QuestIsAccepted(int32 NPCID, int32 QuestID);
	void QuestIsCleared(int32 NPCID, int32 QuestID);

private:
	class UQuestData* GetQuestData(int32 QuestID) const;

public:
	UPROPERTY()
	class UWidget_Quest* QuestWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UListView* QuestList;

};
