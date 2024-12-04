// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UInventoryTooltip;
class UDragItemVisual;
class UItemBase;
class UBorder;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ETHERIA_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; };
protected:
	/// <summary>	
	/// Variables
	/// </summary>
	
	// �巡�� �� ����� �� �׸� ������ �ϳ� ������. �ð��� ������Ҹ� ������.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot");
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	// ������ ���� �ð��� ���
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot");
	TSubclassOf<UInventoryTooltip> ToolTipClass;
	// ������ ������
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UItemBase* ItemReference;
	// �׵θ�(���� ���� ��)
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget));
	UBorder* ItemBorder;
	// ������ 
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget));
	UImage* ItemIcon;
	// ����ǥ���� �ؽ�Ʈ
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget));
	UTextBlock* ItemQuantity;

	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	  // ���콺 ��ư�� ������ �� ��ȣ�ۿ�.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;	// �ݱ�� ���� ��ư�� ������ ���콺��ư�� ���� ������ �ǰԲ�.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;	// �巡�� ����
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;	   // ���� ��ǰ�� ��ĥ�� -> �巡�׷� ������ ���� ���� �Ǻ��� ������ ����.
};
