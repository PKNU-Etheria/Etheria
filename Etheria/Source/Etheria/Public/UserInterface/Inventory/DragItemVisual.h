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
	// �׵θ�(���� ���� ��)
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget));
	UBorder* ItemBorder;
	// ������ 
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget));
	UImage* ItemIcon;
	// ����ǥ���� �ؽ�Ʈ
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget));
	UTextBlock* ItemQuantity;
};
