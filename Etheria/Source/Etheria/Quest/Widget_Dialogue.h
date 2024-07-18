// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EtheriaStructs.h"
#include "Widget_Dialogue.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UWidget_Dialogue : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowDialogue(const FDialogueStruct& DialogueInfo);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetDialogueText(const FText& InText);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Text_NPCName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Text_Dialogue;
};
