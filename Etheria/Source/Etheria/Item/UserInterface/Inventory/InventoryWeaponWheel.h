// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWeaponWheel.generated.h"

class AEPlayer;
class UInventoryWeaponSlot;

UCLASS()
class ETHERIA_API UInventoryWeaponWheel : public UUserWidget
{
	GENERATED_BODY()
public:
	/// <summary>
	/// Functions
	/// </summary>
	void SettingSectionImage();

	void RefreshSectioin();

	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY()
	AEPlayer* PlayerCharacter;
	UPROPERTY()
	class UWeaponWheelComponent* WeaponWheelReference;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWeaponSlot> InventoryWeaponWheelSlotClass;

protected:
	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel", meta = (AllowPrivateAccess = "true"))
	class UMaterialInstanceDynamic* RadialMenuMat;

	// WeaponWheel�� ǥ���ϴ� �̹��� (UI���� ������ ���)
	UPROPERTY(meta = (BindWidget))
	class UImage* InventoryWeaponWheel;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* InventoryWeaponSlotPanel;

	UPROPERTY(meta = (BindWidget))
	class UInventoryWeaponSlot* SwordSlot;

	UPROPERTY(meta = (BindWidget))
	class UInventoryWeaponSlot* SpearSlot;

	UPROPERTY(meta = (BindWidget))
	class UInventoryWeaponSlot* GauntletSlot;

	UPROPERTY(meta = (BindWidget))
	class UInventoryWeaponSlot* CaneSlot;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel")
	TArray<TObjectPtr<UInventoryWeaponSlot>> InventoryWeaponSlot;
};
