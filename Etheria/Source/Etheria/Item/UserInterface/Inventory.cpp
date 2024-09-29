// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Character/Player/EPlayer.h"
#include "Components/InventoryComponent.h"
#include "Components/Button.h"
#include "Item/Item/ItemBase.h"
#include "Item/UserInterface/Inventory/ItemDragDropOperation.h"
#include "Item/UserInterface/Inventory/InventoryWeaponWheel.h"


void UInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
	}

	EquipmentButton->OnClicked.AddDynamic(this, &UInventory::OnEquipmentButtonClicked);
	ConsumableButton->OnClicked.AddDynamic(this, &UInventory::OnConsumableButtonClicked);
	IngredientButton->OnClicked.AddDynamic(this, &UInventory::OnIngredientButtonClicked);
	QuestButton->OnClicked.AddDynamic(this, &UInventory::OnQuestButtonClicked);
}

void UInventory::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
}

bool UInventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryPanel Fail"));
	return false;
}

void UInventory::OnEquipmentButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Weapon);

	InventoryWeaponWheel->SetVisibility(ESlateVisibility::Visible);
}

void UInventory::OnConsumableButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Consumable);

	InventoryWeaponWheel->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventory::OnIngredientButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Ingredient);

	InventoryWeaponWheel->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventory::OnQuestButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Quest);

	InventoryWeaponWheel->SetVisibility(ESlateVisibility::Collapsed);
}
