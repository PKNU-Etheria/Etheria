// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "Items/ItemBase.h"
#include "Pickup.generated.h"

class UDataTable;

UCLASS()
class ETHERIA_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	///	<summary>
	///	Functions;
	///	<summary>
	// Sets default values for this actor's properties
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ACharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	void TakePickup(const ACharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	// 편집기에서 수정한 내용을 즉시 적용하는 이벤트.
#endif

protected:
	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	///	<summary>
	///	Functions;
	///	<summary>
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
