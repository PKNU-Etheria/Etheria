// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EtheriaStructs.h"
#include "EtheriaCharacter.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UWeaponBase : public UObject
{
	GENERATED_BODY()
public:
	/// <summary>
	/// Properties & Variables
	/// </summary>

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UWeaponWheelComponent* OwningWeaponComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UItemBase* ItemReference;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FName ID;	// 기본 데이터 테이블 검색 필드 역할

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	EWeaponType WeaponType;

	/// <summary>
	/// Functions
	/// </summary>
	UWeaponBase();

	void ResetItemFlags();

	UWeaponBase* CreateItemCopy() const;

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
