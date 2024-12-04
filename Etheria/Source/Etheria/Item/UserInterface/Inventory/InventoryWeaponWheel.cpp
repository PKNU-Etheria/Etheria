// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWeaponWheel.h"
#include "ItemDragDropOperation.h"
#include "Character/Player/EPlayer.h"
#include "Item/Item/ItemBase.h"
#include "Item/UserInterface/Inventory/InventoryWeaponSlot.h"
#include "Components/InventoryComponent.h"
#include "Components/WeaponWheelComponent.h"

void UInventoryWeaponWheel::SettingSectionImage()
{
}

void UInventoryWeaponWheel::RefreshSectioin()
{
	for (int i = 0; i < InventoryWeaponSlots.Num(); i++)
	{
		InventoryWeaponSlots[i]->SetItemReference(WeaponWheelReference->GetInventoryContents()[i]);
		InventoryWeaponSlots[i]->UpdateData();
	}
}

void UInventoryWeaponWheel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		WeaponWheelReference = PlayerCharacter->GetWeaponWheel();
		WeaponWheelReference->OnWeaponWheelWidgetUpdated.AddUObject(this, &UInventoryWeaponWheel::RefreshSectioin);
		WeaponWheelReference->OnWeaponWheelUpdated.AddUObject(this, &UInventoryWeaponWheel::RefreshSectioin);
	}

	InventoryWeaponSlots.Add(SwordSlot);
	SwordSlot->SetSlotType(EWeaponType::Sword);
	InventoryWeaponSlots.Add(SpearSlot);
	SpearSlot->SetSlotType(EWeaponType::Spear);
	InventoryWeaponSlots.Add(GauntletSlot);
	GauntletSlot->SetSlotType(EWeaponType::Gauntlet);
	InventoryWeaponSlots.Add(BowSlot);
	BowSlot->SetSlotType(EWeaponType::Bow);
}

bool UInventoryWeaponWheel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && WeaponWheelReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryWeaponWheel"));

		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryWeaponWheel Fail!!!!!"));
	return false;
}
