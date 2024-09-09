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
	
	// 드래그 앤 드롭할 때 항목 슬롯이 하나 생성됨. 시각적 구성요소만 보관됨.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot");
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	// 아이템 툴팁 시각적 요소
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot");
	TSubclassOf<UInventoryTooltip> ToolTipClass;
	// 아이템 데이터
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UItemBase* ItemReference;
	// 테두리(색상 변경 등)
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget));
	UBorder* ItemBorder;
	// 아이콘 
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget));
	UImage* ItemIcon;
	// 수량표시할 텍스트
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget));
	UTextBlock* ItemQuantity;

	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	  // 마우스 버튼을 눌렸을 때 상호작용.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;	// 닫기와 같은 버튼을 누를때 마우스버튼을 때야 적용이 되게끔.
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;	// 드래그 감지
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;	   // 같은 물품을 합칠때 -> 드래그로 쌓을때 같은 유형 판별후 스택을 조정.
};
