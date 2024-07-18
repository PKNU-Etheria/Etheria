// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/Widget_Dialogue.h"
#include "Components/TextBlock.h"

void UWidget_Dialogue::ShowDialogue(const FDialogueStruct& DialogueInfo)
{
	Text_NPCName->SetText(FText::FromName(DialogueInfo.NPCName));
	SetDialogueText(DialogueInfo.Script);
}
