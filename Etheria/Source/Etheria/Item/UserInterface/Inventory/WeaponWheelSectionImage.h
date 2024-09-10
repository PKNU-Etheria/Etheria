// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWheelSectionImage.generated.h"

class UImage;
class UItemBase;

UCLASS()
class ETHERIA_API UWeaponWheelSectionImage : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; };

	FORCEINLINE void SetWeaponIcon(UImage* ImageIn) { WeaponIcon = ImageIn; };
	FORCEINLINE UImage* GetWeaponIcon() const { return WeaponIcon; };

protected:
	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	/// <summary>	
	/// Variables
	/// </summary>
	// Item Data
	UPROPERTY(VisibleAnywhere, Category = "WeaponWheelSectionSlot")
	UItemBase* ItemReference;
	// Item Inventory Icon 
	UPROPERTY(VisibleAnywhere, Category = "WeaponWheelSectionSlot", meta = (BindWidget));
	UImage* WeaponIcon;
};
