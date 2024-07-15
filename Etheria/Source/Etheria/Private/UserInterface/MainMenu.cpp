// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/MainMenu.h"
#include "World/ItemTestCharacter.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AItemTestCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;

	// cast operation to item drag drop, ensure player is valid, call drop item on player
}
