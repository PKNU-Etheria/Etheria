// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Quest.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UWidget_Quest : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void ShowQuests();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget_QuestList* QuestList_Startable;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget_QuestList* QuestList_Progressing;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWidget_QuestList* QuestList_Cleared;
};
