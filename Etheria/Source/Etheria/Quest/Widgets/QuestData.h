// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EtheriaStructs.h"
#include "QuestData.generated.h"

UCLASS()
class ETHERIA_API UQuestData : public UObject
{
	GENERATED_BODY()

public:
	UQuestData() {}

	const FQuestStruct* QuestInfo;
	int QuestID = 0;
};
