// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/Inventory/WeaponWheelSectionImage.h"
#include "Components/Image.h"
#include "Public/Items/ItemBase.h"

void UWeaponWheelSectionImage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UWeaponWheelSectionImage::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (ItemReference)
	{
		WeaponIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
	}*/
}
