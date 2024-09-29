// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/UserInterface/Inventory/InventoryWeaponSlot.h"
#include "Item/UserInterface/Inventory/InventoryTooltip.h"
#include "Components/Border.h"
#include "Components/Image.h"


void UInventoryWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		ItemBorder->SetBrushColor(FLinearColor::White);
	}

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
}

bool UInventoryWeaponSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
