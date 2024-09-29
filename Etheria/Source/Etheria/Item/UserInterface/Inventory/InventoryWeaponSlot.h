// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/UserInterface//Inventory/InventoryItemSlot.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWeaponSlot.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UInventoryWeaponSlot : public UInventoryItemSlot
{
	GENERATED_BODY()
public:


protected:
	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	  // 마우스 버튼을 눌렸을 때 상호작용.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;	// 닫기와 같은 버튼을 누를때 마우스버튼을 때야 적용이 되게끔.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;	// 드래그 감지
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;	   // 같은 물품을 합칠때 -> 드래그로 쌓을때 같은 유형 판별후 스택을 조정.
	
	/// <summary>	
	/// Variables
	/// </summary>

};
