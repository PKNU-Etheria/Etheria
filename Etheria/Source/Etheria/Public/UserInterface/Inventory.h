// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

class AEPlayer;
class UInventoryComponent;
class UInventoryWeaponWheel;
/**
 * 
 */
UCLASS()
class ETHERIA_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AEPlayer* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;


protected:
	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnEquipmentButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnConsumableButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnIngredientButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnQuestButtonClicked();

	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* EquipmentButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* ConsumableButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* IngredientButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuestButton;

	UPROPERTY(VisibleAnywhere, Category = "InventoryWeaponWheel", meta = (BindWidget));
	UInventoryWeaponWheel* InventoryWeaponWheel;

	// ConSumable Item Setting
};
