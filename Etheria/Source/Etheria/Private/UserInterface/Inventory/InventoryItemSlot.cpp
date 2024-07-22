// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Public/UserInterface/Inventory/InventoryTooltip.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Public/Items/ItemBase.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ToolTipClass)
	{
		UInventoryTooltip* ToolTip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{	// 아이템 품질에 따른 색상
		switch (ItemReference->ItemQuality)
		{
		case EItemQuality::Normal:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::Rear:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		case EItemQuality::Epic:
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::Legend:
			ItemBorder->SetBrushColor(FLinearColor::Yellow);
			break;
		case EItemQuality::Special:
			ItemBorder->SetBrushColor(FLinearColor(100.0f, 65.0f, 0.0f, 1.0f));
			break;
		default:
			break;
		}
		// 아이콘 세팅
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{	// 스택이지 않은 아이템은 수량을 보여줄 필요없음. (장비같은)
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

//FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	return nullptr;
//}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
