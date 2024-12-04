// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/UserInterface/Inventory/InventoryWeaponSlot.h"
#include "Item/UserInterface/Inventory/InventoryTooltip.h"
#include "Item/Item/Pickup.h"
#include "DragItemVisual.h"
#include "ItemDragDropOperation.h"
#include "Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Character/Player/EPlayer.h"
#include "Input/Reply.h"
#include "Input/Events.h"

void UInventoryWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());

	if (ItemReference)
	{
		ItemBorder->SetBrushColor(FLinearColor::Black);
	}
	// 아이콘 세팅
	if (ItemReference)
	{
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
	}

	ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UInventoryWeaponSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	// submenu on right click will happen here

	return Reply.Unhandled();
}

void UInventoryWeaponSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UInventoryWeaponSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemReference->NumericData.bIsStackable ? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity)) : DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;
		DragItemOperation->bIsEquip = true;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft; // 마우스에 부착되는 위치

		OutOperation = DragItemOperation;
	}
}

bool UInventoryWeaponSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryWeaponSlot"));

		if (PlayerCharacter->GetInventory()->FindMatchingItem(ItemDragDrop->SourceItem))
		{
			// 만약 아이템이 인벤토리에 존재한다면? 제거해야지
			PlayerCharacter->GetInventory()->RemoveSingleInstanceOfItem(ItemDragDrop->SourceItem);
		}

		ItemReference = ItemDragDrop->SourceItem;

		UpdateData();

		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryWeaponSlot Fail!!!!!"));
	return false;
}
