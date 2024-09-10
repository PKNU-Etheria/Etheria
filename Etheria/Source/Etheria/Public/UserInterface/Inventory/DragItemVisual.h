// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

class UBorder;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ETHERIA_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 테두리(색상 변경 등)
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget));
	UBorder* ItemBorder;
	// 아이콘 
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget));
	UImage* ItemIcon;
	// 수량표시할 텍스트
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget));
	UTextBlock* ItemQuantity;
};
