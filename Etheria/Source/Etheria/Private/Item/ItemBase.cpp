// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "Public/Components/InventoryComponent.h"

UItemBase::UItemBase()
{
}

UItemBase* UItemBase::CreateItemCopy() const
{	// 인벤토에 아이템을 추가할 때 복사. 새로운 객체를 생성하고 정적 클래스를 얻음.
	UItemBase* ItemCopy = NewObject <UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		//  하나의 스택이 다른 스택에 병합되거나 삭제될 때 - 수량을 변경할 때마다 필터링(setQAuantity 함수를 통해)
		if (OwningInventory)
		{
			if(Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
	}

}

void UItemBase::Use(AEtheriaCharacter* Character)
{

}
