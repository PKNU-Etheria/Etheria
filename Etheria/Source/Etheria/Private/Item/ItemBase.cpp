// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

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
}

void UItemBase::Use(AEtheriaCharacter* Character)
{
}
