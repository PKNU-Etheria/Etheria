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

	CurrentInventoryType = EItemType::Weapon;
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
	{	// 인벤토리에 해당 아이템이 있으면 반환.
		TArray<TObjectPtr<UItemBase>> InventoryContents;

		switch (ItemIn->ItemType)
		{
		case EItemType::Quest:
			InventoryContents = QuestInventoryContents;
			break;
		case EItemType::Consumable:
			InventoryContents = ConsumeInventoryContents;
			break;
		case EItemType::Ingredient:
			InventoryContents = IngredientInventoryContents;
			break;
		case EItemType::Weapon:
			InventoryContents = WeaponInventoryContents;
			break;
		}

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
	{	// 인벤토리에 해당 아이디를 가진 아이템이 있으면 반환. 이중 포인터 사용. 포인터를 포인터로 가리킴.
		TArray<TObjectPtr<UItemBase>> InventoryContents;

		switch (ItemIn->ItemType)
		{
		case EItemType::Quest:
			InventoryContents = QuestInventoryContents;
			break;
		case EItemType::Consumable:
			InventoryContents = ConsumeInventoryContents;
			break;
		case EItemType::Ingredient:
			InventoryContents = IngredientInventoryContents;
			break;
		case EItemType::Weapon:
			InventoryContents = WeaponInventoryContents;
			break;
		}

		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}

	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{	// 이미 인벤토리에 존재하는 아이템으로 체크할 필요 없음.
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	switch (ItemIn->ItemType)
	{
	case EItemType::Quest:
		InventoryContents = QuestInventoryContents;
		break;
	case EItemType::Consumable:
		InventoryContents = ConsumeInventoryContents;
		break;
	case EItemType::Ingredient:
		InventoryContents = IngredientInventoryContents;
		break;
	case EItemType::Weapon:
		InventoryContents = WeaponInventoryContents;
		break;
	}

	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{	// ItemIn에 대한 정보를 인벤토리 아이템들과 비교 for루프문 대신 람다형식이용
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
	switch (ItemToRemove->ItemType)
	{
	case EItemType::Quest:
		QuestInventoryContents.RemoveSingle(ItemToRemove);
		break;
	case EItemType::Consumable:
		ConsumeInventoryContents.RemoveSingle(ItemToRemove);
		break;
	case EItemType::Ingredient:
		IngredientInventoryContents.RemoveSingle(ItemToRemove);
		break;
	case EItemType::Weapon:
		WeaponInventoryContents.RemoveSingle(ItemToRemove);
		break;
	}

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
{	// 인벤토리 내용물의 총 개수가 인벤토리 슬롯 용량보다 작거나 같다면.
	// 인벤토리 안에 있는 내용물. 픽업 X
	// 추후에 인벤토리 용량개수 제한을 없애든가 고민해봐야 할 듯
	int32 ContentsNum = QuestInventoryContents.Num() + ConsumeInventoryContents.Num() + IngredientInventoryContents.Num() + WeaponInventoryContents.Num();
	if (!(ContentsNum + 1 > InventorySlotsCapacity))
	{	// 내용물을 쪼개서 넣는다.
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem, int32 RequestedAddAmount)
{
	// InputItem의 weight가 유효한지 체크.
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. item has invalid weight value."), InputItem->TextData.Name));
	}
	// 무게가 초과 될 경우.
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. item would overflow weight limit."), InputItem->TextData.Name));
	}
	// 공간이 초과될 경우.
	// 추후에 인벤토리 용량개수 제한을 없애든가 고민해봐야 할 듯
	int32 ContentsNum = QuestInventoryContents.Num() + ConsumeInventoryContents.Num() + IngredientInventoryContents.Num() + WeaponInventoryContents.Num();
	if (ContentsNum + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	// 아이템 추가
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

	int32 AmountToDistribute = RequestedAddAmount;	// 인벤토리에 넣기 위한 남은 개수

	// 넣을 아이템이 인벤토리에 이미 존재할 경우와 개수가 최대 개수가 아닐경우.
	UItemBase* ExistingItemStack = FindNextPartialStack(InputItem);

	// distribute item stack over existing stacks
	while (ExistingItemStack)
	{	// 풀 스택까지 남은 개수 가져오기. (들어갈 수 있는 허용 값)
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		//  무게 용량 고려하기. 풀 스택까지 남은 양 중에서 실제로 넣을 수 있는 양.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

		// 추가할 수 있는 양이 허용하는 용량을 넘지 않는 경우. 추가 가능한 무게가 있을 경우
		if (WeightLimitAddAmount > 0)
		{	// 인벤토리에 있던 기존 아이템 수량 변경 및 인벤토리 무게 변경.
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);
			// 인벤에 추가한 스택 값만큼 주운 아이템 수량을 다시 조정해주기 위해 연산
			AmountToDistribute -= WeightLimitAddAmount;
			// 주운 아이템 수량 다시 조절.
			InputItem->SetQuantity(AmountToDistribute);

			// 아이템을 추가했을 때 최대 용량이 초과할 경우
			if (InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{	// 총 용량이 인벤토리 현재 용량과 같다면. 즉, 최대 용량에 도달했다면 루프 탈출.
				OnInventoryUpdated.Broadcast();	// 인벤 업데이트
				return RequestedAddAmount - AmountToDistribute;	// 들어간 개수
			}
		}
		else if(WeightLimitAddAmount <= 0)	// 만약 허용되는 개수가 0이라면
		{	// 남은 스택은 있지만 인벤토리가 허용할 수 있는 무게 용량이 찼을 경우.
			// 아이템을 추가할 수 있는 용량은 허용되지 않음. 추가되는 것을 막기.
			if (AmountToDistribute != RequestedAddAmount)
			{	
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		if (AmountToDistribute <= 0)	// 용량도 개수도 모두 다 스택 가능
		{	// 모든 아이템은 기존 스택 아이템에 추가됨.
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}
		// 여전히 쌓을 수 있는 공간이 있을 수도 있기 때문에 한번더 체크.
		ExistingItemStack = FindNextPartialStack(InputItem);
	}

	// 쌓을 수 없다면 새로운 인벤토리 슬롯을 만들어야함. 공간이 있는지 체크.
	// 추후에 인벤토리 용량개수 제한을 없애든가 고민해봐야 할 듯
	int32 ContentsNum = QuestInventoryContents.Num() + ConsumeInventoryContents.Num() + IngredientInventoryContents.Num() + WeaponInventoryContents.Num();
	if (ContentsNum + 1 <= InventorySlotsCapacity)
	{	// 스택에 여유가 없어서 새로운 공간에 해당 아이템을 새롭게 만들 수 있느냐?
		// 공간이 있을 경우. 남은 아이템 수량을 추가할 공간이 있는지 확인.
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(InputItem, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{	// 추가할 수 있는 양이 전체 양보다 작을 경우.
			if (WeightLimitAddAmount < AmountToDistribute)
			{	// 일부만 추가할 수 있도록 한다.
				AmountToDistribute -= WeightLimitAddAmount;
				InputItem->SetQuantity(AmountToDistribute);

				// 추가할 수 있는 수량만큼 아이템을 새롭게 만들어서 복제한다.
				AddNewItem(InputItem->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}
			// 아이템의 모든 수량을 추가할 수 있을 경우. 다 추가한다.
			AddNewItem(InputItem, AmountToDistribute);
			return RequestedAddAmount;
		}
		// 아이템 슬롯은 여유가 있지만 용량이 부족할 경우.
		return RequestedAddAmount - AmountToDistribute;
	}

	// 쌓을 수 없고 추가 용량 슬롯이 존재하지 않을 경우.
	return 0;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{	// 아이템이 인벤토리에 들어올 때마다 확인하는 시작점.
	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Handle AddItem Start"));
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// 스택이 없는 아이템
		if (!InputItem->NumericData.bIsStackable)
		{
			UE_LOG(LogTemp, Warning, TEXT("Check NonStackable Start"));
			return HandleNonStackableItems(InputItem, InitialRequestedAddAmount);
		}

		// 스택인 아이템
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);	 // 추가되는 양

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			UE_LOG(LogTemp, Warning, TEXT("Check Stackable 1 Start"));
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully add {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Check Stackable 2 Start"));
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} added to the inventory. Number add = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Check Stackable 3 Start"));
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
	{	// 사본이거나 월드에서 픽업일 경우.
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{	// 기존 아이템을 스택 분할하거나 다른 인벤토리 공간으로 드래깅(이동)할 경우
		NewItem = Item->CreateItemCopy();
	}


	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	switch (NewItem->ItemType)
	{
		case EItemType::Quest:
			QuestInventoryContents.Add(NewItem);
			break;
		case EItemType::Consumable:
			ConsumeInventoryContents.Add(NewItem);
			break;
		case EItemType::Ingredient:
			IngredientInventoryContents.Add(NewItem);
			break;
		case EItemType::Weapon:
			WeaponInventoryContents.Add(NewItem);
			break;
	}

	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("AddNewItem Clear"));
}
