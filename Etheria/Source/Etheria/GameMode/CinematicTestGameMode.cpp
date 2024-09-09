// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CinematicTestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "Engine/DataTable.h"

void ACinematicTestGameMode::BossCleared()
{
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!player) return;

	// Give Item
	UInventoryComponent* InvComp = player->GetComponentByClass<UInventoryComponent>();
	if (InvComp)
	{
		const FItemData* ItemData = ItemDB->FindRow<FItemData>(RewardItemID, RewardItemID.ToString());

		UItemBase* item = NewObject<UItemBase>(this, UItemBase::StaticClass());

		item->ID = ItemData->ID;
		item->ItemType = ItemData->ItemType;
		item->ItemQuality = ItemData->ItemQuality;
		item->NumericData = ItemData->NumericData;
		item->TextData = ItemData->TextData;
		item->AssetData = ItemData->AssetData;
		item->Quantity = 1;

		item->NumericData.bIsStackable = (ItemData->NumericData.MaxStackSize > 1);
		
		InvComp->HandleAddItem(item);
	}

	// Play Cinematic (Sequence)
	PlayBossClearCinematic();
}
