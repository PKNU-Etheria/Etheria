// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

UWeaponBase::UWeaponBase()
{
}

UWeaponBase* UWeaponBase::CreateItemCopy() const
{
	UWeaponBase* ItemCopy = NewObject<UWeaponBase>(StaticClass());

	ItemCopy->ItemReference = this->ItemReference;

	ItemCopy->ID = this->ID;

	ItemCopy->WeaponType = this->WeaponType;

	return ItemCopy;
}
