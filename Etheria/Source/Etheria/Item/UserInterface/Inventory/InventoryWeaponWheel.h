// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWeaponWheel.generated.h"

class AEPlayer;
class UInventoryComponent;

UCLASS()
class ETHERIA_API UInventoryWeaponWheel : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	AEPlayer* PlayerCharacter;
	UPROPERTY()
	UInventoryComponent* InventoryReference;

protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
