// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWeaponWheel.h"
#include "ItemDragDropOperation.h"
#include "Character/Player/EPlayer.h"
#include "Components/InventoryComponent.h"

void UInventoryWeaponWheel::SettingSectionImage()
{
}

void UInventoryWeaponWheel::RefreshSectioin()
{

}

void UInventoryWeaponWheel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AEPlayer>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		WeaponWheelReference = PlayerCharacter->GetWeaponWheel();
		WeaponWheelReference->OnWeaponWheelWidgetUpdated.AddUObject(this, &UWeaponWheel::UpdateActiveSection);
		WeaponWheelReference->OnWeaponWheelUpdated.AddUObject(this, &UWeaponWheel::RefreshSection);
	}

	InventoryWeaponSlot.Add(SwordSlot);
	InventoryWeaponSlot.Add(SpearSlot);
	InventoryWeaponSlot.Add(GauntletSlot);
	InventoryWeaponSlot.Add(CaneSlot);
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
