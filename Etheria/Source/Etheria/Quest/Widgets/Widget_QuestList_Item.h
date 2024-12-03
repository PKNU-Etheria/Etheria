// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "EtheriaEnums.h"
#include "EtheriaStructs.h"
#include "Widget_QuestList_Item.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UWidget_QuestList_Item : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestID;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestName;

protected:
	UPROPERTY(BlueprintReadOnly)
	EQuestDetailType QuestType;

	UPROPERTY(BlueprintReadOnly)
	int QuestID = -1;

	UPROPERTY(BlueprintReadOnly)
	FName QuestName;

	UPROPERTY(BlueprintReadOnly)
	FText QuestDescription;

protected:
	const FQuestStruct* QuestData;
};
