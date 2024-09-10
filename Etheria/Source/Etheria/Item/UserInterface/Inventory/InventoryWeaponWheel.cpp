// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/InventoryWeaponWheel.h"
#include "Public/UserInterface/Inventory/ItemDragDropOperation.h"
#include "Character/Player/EPlayer.h"
#include "Components/InventoryComponent.h"

void UInventoryWeaponWheel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
	}
}

bool UInventoryWeaponWheel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryWeaponWheel"));

		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Detected and item drop on InventoryWeaponWheel Fail!!!!!"));
	return false;
}
