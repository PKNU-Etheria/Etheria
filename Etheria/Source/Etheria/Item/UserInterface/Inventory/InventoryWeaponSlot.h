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
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	  // ���콺 ��ư�� ������ �� ��ȣ�ۿ�.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;	// �ݱ�� ���� ��ư�� ������ ���콺��ư�� ���� ������ �ǰԲ�.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;	// �巡�� ����
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;	   // ���� ��ǰ�� ��ĥ�� -> �巡�׷� ������ ���� ���� �Ǻ��� ������ ����.
	
	/// <summary>	
	/// Variables
	/// </summary>

};
