// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Widgets/Widget_QuestList.h"
#include "Quest/Widgets/Widget_Quest.h"
#include "Quest/Widgets/Widget_QuestList_Item.h"
#include "Components/ListView.h"
#include "Quest/Widgets/QuestData.h"
#include "Quest/QuestSubSystem.h"

void UWidget_QuestList::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInst = GetGameInstance();
	if (!GameInst) return;
	UQuestSubSystem* QuestSystem = GameInst->GetSubsystem<UQuestSubSystem>();
	if (QuestSystem)
	{
		QuestSystem->Available_Delegate.AddUObject(this, &UWidget_QuestList::QuestIsAvailable);
		QuestSystem->Clearable_Delegate.AddUObject(this, &UWidget_QuestList::QuestIsClearable);
		QuestSystem->Accept_Delegate.AddUObject(this, &UWidget_QuestList::QuestIsAccepted);
		QuestSystem->Clear_Delegate .AddUObject(this, &UWidget_QuestList::QuestIsCleared);
	}
}

void UWidget_QuestList::SetQuestWidget(UWidget_Quest* parent)
{
	QuestWidget = parent;
}

void UWidget_QuestList::AddUnAvailableQuest(const FQuestStruct* InQuestInfo)
{
	UQuestData* data = NewObject<UQuestData>(UQuestData::StaticClass());
	data->QuestType = EQuestDetailType::EQDT_Startable;
	data->QuestID = InQuestInfo->QuestID;
	data->QuestInfo = InQuestInfo;
	QuestList->AddItem(data);
}

void UWidget_QuestList::AddAvailableQuest(const FQuestStruct* InQuestInfo)
{
	UQuestData* data = NewObject<UQuestData>(UQuestData::StaticClass());
	data->QuestType = EQuestDetailType::EQDT_Startable;
	data->QuestID = InQuestInfo->QuestID;
	data->QuestInfo = InQuestInfo;
	QuestList->AddItem(data);
}

void UWidget_QuestList::AddProgressingQuest(const FQuestStruct* InQuestInfo)
{
	UQuestData* data = NewObject<UQuestData>(UQuestData::StaticClass());
	data->QuestType = EQuestDetailType::EQDT_Progressing;
	data->QuestID = InQuestInfo->QuestID;
	data->QuestInfo = InQuestInfo;
	QuestList->AddItem(data);
}

void UWidget_QuestList::AddClearableQuest(const FQuestStruct* InQuestInfo)
{
	UQuestData* data = NewObject<UQuestData>(UQuestData::StaticClass());
	data->QuestType = EQuestDetailType::EQDT_Progressing;
	data->QuestID = InQuestInfo->QuestID;
	data->QuestInfo = InQuestInfo;
	QuestList->AddItem(data);
}

void UWidget_QuestList::AddClearedQuest(const FQuestStruct* InQuestInfo)
{
	UQuestData* data = NewObject<UQuestData>(UQuestData::StaticClass());
	data->QuestType = EQuestDetailType::EQDT_Cleared;
	data->QuestID = InQuestInfo->QuestID;
	data->QuestInfo = InQuestInfo;
	QuestList->AddItem(data);
}

void UWidget_QuestList::QuestIsAvailable(int32 NPCID, int32 QuestID)
{
	UQuestData* QuestData = GetQuestData(QuestID);
	if (!QuestData) return;

	UUserWidget* entryWidget = QuestList->GetEntryWidgetFromItem(QuestData);
	if (!entryWidget) return;

	UWidget_QuestList_Item* QuestList_Item = Cast<UWidget_QuestList_Item>(entryWidget);
	if (QuestList_Item)
	{

	}
}

void UWidget_QuestList::QuestIsClearable(int32 NPCID, int32 QuestID)
{
	UQuestData* QuestData = GetQuestData(QuestID);
	if (!QuestData) return;

	UUserWidget* entryWidget = QuestList->GetEntryWidgetFromItem(QuestData);
	if (!entryWidget) return;

	UWidget_QuestList_Item* QuestList_Item = Cast<UWidget_QuestList_Item>(entryWidget);
	if (QuestList_Item)
	{

	}

}

void UWidget_QuestList::QuestIsAccepted(int32 NPCID, int32 QuestID)
{
	UQuestData* QuestData = GetQuestData(QuestID);
	if (!QuestData) return;

	QuestList->RemoveItem(QuestData);

	if (QuestWidget)
	{
		QuestWidget->QuestList_Progressing->AddProgressingQuest(QuestData->QuestInfo);
	}

}

void UWidget_QuestList::QuestIsCleared(int32 NPCID, int32 QuestID)
{
	UQuestData* QuestData = GetQuestData(QuestID);
	if (!QuestData) return;

	QuestList->RemoveItem(QuestData);

	if (QuestWidget)
	{
		QuestWidget->QuestList_Cleared->AddClearedQuest(QuestData->QuestInfo);
	}
}

UQuestData* UWidget_QuestList::GetQuestData(int32 QuestID) const
{
	UQuestData* QuestData = nullptr;

	const TArray<UObject*> ItemArray = QuestList->GetListItems();
	for (int i = 0; i < ItemArray.Num(); i++)
	{
		UQuestData* QuestItem = Cast<UQuestData>(ItemArray[i]);
		if (QuestItem && QuestItem->QuestID == QuestID)
		{
			QuestData = QuestItem;
			break;
		}
	}

	return QuestData;
}
