// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Components/InventoryComponent.h"
#include "Public/Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn)
	{	// �κ��丮�� �ش� �������� ������ ��ȯ.
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}

	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{	// �κ��丮�� �ش� ���̵� ���� �������� ������ ��ȯ. ���� ������ ���. �����͸� �����ͷ� ����Ŵ.
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}

	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{	// �̹� �κ��丮�� �����ϴ� ���������� üũ�� �ʿ� ����.
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{	// ItemIn�� ���� ������ �κ��丮 �����۵�� �� for������ ��� ���������̿�
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}) 
		)
	{
		return *Result;
	}

	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);

	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();
	
	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{	// �κ��丮 ���빰�� �� ������ �κ��丮 ���� �뷮���� �۰ų� ���ٸ�.
	// �κ��丮 �ȿ� �ִ� ���빰. �Ⱦ� X
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{	// ���빰�� �ɰ��� �ִ´�.
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem, int32 RequestedAddAmount)
{
	// InputItem�� weight�� ��ȿ���� üũ.
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. item has invalid weight value."), InputItem->TextData.Name));
	}
	// ���԰� �ʰ� �� ���.
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. item would overflow weight limit."), InputItem->TextData.Name));
	}
	// ������ �ʰ��� ���.
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	// ������ �߰�
	AddNewItem(InputItem, RequestedAddAmount);
	return FItemAddResult::AddedAll(RequestedAddAmount, FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), RequestedAddAmount, InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* InputItem, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(InputItem->GetItemStackWeight()))
	{
		// invalid item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;	// �κ��丮�� �ֱ� ���� ���� ����

	// ���� �������� �κ��丮�� �̹� ������ ���� ������ �ִ� ������ �ƴҰ��.
	UItemBase* ExistingItemStack = FindNextPartialStack(InputItem);

	// distribute item stack over existing stacks
	while (ExistingItemStack)
	{	// Ǯ ���ñ��� ���� ���� ��������. (�� �� �ִ� ��� ��)
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		//  ���� �뷮 ����ϱ�. Ǯ ���ñ��� ���� �� �߿��� ������ ���� �� �ִ� ��.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// �߰��� �� �ִ� ���� ����ϴ� �뷮�� ���� �ʴ� ���. �߰� ������ ���԰� ���� ���
		if (WeightLimitAddAmount > 0)
		{	// �κ��丮�� �ִ� ���� ������ ���� ���� �� �κ��丮 ���� ����.
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);
			// �κ��� �߰��� ���� ����ŭ �ֿ� ������ ������ �ٽ� �������ֱ� ���� ����
			AmountToDistribute -= WeightLimitAddAmount;
			// �ֿ� ������ ���� �ٽ� ����.
			InputItem->SetQuantity(AmountToDistribute);

			// �������� �߰����� �� �ִ� �뷮�� �ʰ��� ���
			if (InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{	// �� �뷮�� �κ��丮 ���� �뷮�� ���ٸ�. ��, �ִ� �뷮�� �����ߴٸ� ���� Ż��.
				OnInventoryUpdated.Broadcast();	// �κ� ������Ʈ
				return RequestedAddAmount - AmountToDistribute;	// �� ����
			}
		}
		else if(WeightLimitAddAmount <= 0)	// ���� ���Ǵ� ������ 0�̶��
		{	// ���� ������ ������ �κ��丮�� ����� �� �ִ� ���� �뷮�� á�� ���.
			// �������� �߰��� �� �ִ� �뷮�� ������ ����. �߰��Ǵ� ���� ����.
			if (AmountToDistribute != RequestedAddAmount)
			{	
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if (AmountToDistribute <= 0)	// �뷮�� ������ ��� �� ���� ����
		{	// ��� �������� ���� ���� �����ۿ� �߰���.
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}
		// ������ ���� �� �ִ� ������ ���� ���� �ֱ� ������ �ѹ��� üũ.
		ExistingItemStack = FindNextPartialStack(InputItem);
	}

	// ���� �� ���ٸ� ���ο� �κ��丮 ������ ��������. ������ �ִ��� üũ.
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{	// ���ÿ� ������ ��� ���ο� ������ �ش� �������� ���Ӱ� ���� �� �ִ���?
		// ������ ���� ���. ���� ������ ������ �߰��� ������ �ִ��� Ȯ��.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(InputItem, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{	// �߰��� �� �ִ� ���� ��ü �纸�� ���� ���.
			if (WeightLimitAddAmount < AmountToDistribute)
			{	// �Ϻθ� �߰��� �� �ֵ��� �Ѵ�.
				AmountToDistribute -= WeightLimitAddAmount;
				InputItem->SetQuantity(AmountToDistribute);

				// �߰��� �� �ִ� ������ŭ �������� ���Ӱ� ���� �����Ѵ�.
				AddNewItem(InputItem->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}
			// �������� ��� ������ �߰��� �� ���� ���. �� �߰��Ѵ�.
			AddNewItem(InputItem, AmountToDistribute);
			return RequestedAddAmount;
		}
		// ������ ������ ������ ������ �뷮�� ������ ���.
		return RequestedAddAmount - AmountToDistribute;
	}

	// ���� �� ���� �߰� �뷮 ������ �������� ���� ���.
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{	// �������� �κ��丮�� ���� ������ Ȯ���ϴ� ������.
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// ������ ���� ������
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem, InitialRequestedAddAmount);
		}

		// ������ ������
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);	 // �߰��Ǵ� ��

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully add {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} added to the inventory. Number add = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), InputItem->TextData.Name));
		}
	}

	check(false);

	return FItemAddResult();
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{	// �纻�̰ų� ���忡�� �Ⱦ��� ���.
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{	// ���� �������� ���� �����ϰų� �ٸ� �κ��丮 �������� �巡��(�̵�)�� ���
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}
