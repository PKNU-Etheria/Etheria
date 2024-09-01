// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMenu.h"
#include "Character/Player/EPlayer.h"
#include "Components/InventoryComponent.h"
#include "Components/Button.h"
#include "Public/Items/ItemBase.h"
#include "Public/UserInterface/Inventory/ItemDragDropOperation.h"


void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
	}

	EquipmentButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Equipment")));
	EquipmentButton->OnClicked.AddDynamic(this, &UMainMenu::OnEquipmentButtonClicked);
	ConsumableButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Consumable")));
	ConsumableButton->OnClicked.AddDynamic(this, &UMainMenu::OnConsumableButtonClicked);
	IngredientButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Ingredient")));
	IngredientButton->OnClicked.AddDynamic(this, &UMainMenu::OnIngredientButtonClicked);
	QuestButton = Cast<UButton>(GetWidgetFromName(TEXT("Button_Quest")));
	QuestButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuestButtonClicked);
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

void UMainMenu::OnEquipmentButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Weapon);
}

void UMainMenu::OnConsumableButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Consumable);
}

void UMainMenu::OnIngredientButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Ingredient);
}

void UMainMenu::OnQuestButtonClicked()
{
	InventoryReference->ChangeInventoryType(EItemType::Quest);
}
