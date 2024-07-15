// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EtheriaStructs.h"
#include "EtheriaCharacter.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// Properties & Variables
	/// </summary>
	
	//UPROPERTY()
	//UInventoryComponent* OwningInvectory;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;	// 기본 데이터 테이블 검색 필드 역할

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;

	/// <summary>
	/// Functions
	/// </summary>
	UItemBase();

	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const {
		return Quantity * NumericData.Weight;
	};

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const {
		return NumericData.Weight;
	};

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const {
		return Quantity == NumericData.MaxStackSize;
	};

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AEtheriaCharacter* Character);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
