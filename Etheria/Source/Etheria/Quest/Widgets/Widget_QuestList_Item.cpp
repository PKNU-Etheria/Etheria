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

	UQuestData* QuestData = Cast<UQuestData>(ListItemObject);
	if (QuestData)
	{
		QuestID = QuestData->QuestID;

		// Set Quest Info (Quest Name . . . )
		if (QuestData->QuestInfo)
		{
			Text_QuestID->SetText(UKismetTextLibrary::Conv_IntToText(QuestID));
			Text_QuestName->SetText(FText::FromName(QuestData->QuestInfo->QuestName));
		}
	}
}
