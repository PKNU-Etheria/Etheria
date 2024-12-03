// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Widgets/Widget_QuestList_Item.h"
#include "Kismet/KismetTextLibrary.h"
#include "Components/TextBlock.h"
#include "Quest/Widgets/QuestData.h"
#include "Quest/QuestSubSystem.h"
#include "Kismet/KismetTextLibrary.h"

void UWidget_QuestList_Item::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UGameInstance* GameInst = GetGameInstance();
	if (!GameInst) return;
	UQuestSubSystem* QuestSystem =GameInst->GetSubsystem<UQuestSubSystem>();
	if (QuestSystem)
	{
		// QuestSystem->Available_Delegate.AddUObject(this, )
	}

	UQuestData* questData = Cast<UQuestData>(ListItemObject);

	// Set Quest Info (Quest Name . . . )
	if (questData && questData->QuestInfo)
	{
		Text_QuestID->SetText(UKismetTextLibrary::Conv_IntToText(QuestID));
		Text_QuestName->SetText(FText::FromName(questData->QuestInfo->QuestName));

		this->QuestData = questData->QuestInfo;
		QuestType = questData->QuestType;
		QuestName = questData->QuestInfo->QuestName;
		QuestDescription = questData->QuestInfo->QuestDescription;
		QuestID = questData->QuestID;
	}
}
