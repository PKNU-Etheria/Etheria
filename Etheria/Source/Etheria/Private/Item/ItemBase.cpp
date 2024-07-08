// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

UItemBase::UItemBase()
{
}

UItemBase* UItemBase::CreateItemCopy() const
{	// �κ��信 �������� �߰��� �� ����. ���ο� ��ü�� �����ϰ� ���� Ŭ������ ����.
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
