// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EtheriaEnums.h"
#include "Widget_FloatingNpcDesc.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UWidget_FloatingNpcDesc : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNPCName(FName Name);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNPCStatsus(ENPCState State);
};
