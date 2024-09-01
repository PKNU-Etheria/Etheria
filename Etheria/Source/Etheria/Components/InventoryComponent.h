// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EtheriaEnums.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0), OperationResult(EItemAddResult::IAR_NoItemAdded), ResultMessage(FText::GetEmpty())
	{

	};

	// 인벤토리에 들어가는 아이템 실제 수량. 
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;
	// 아이템 추가 작업시에 나타내는 상태
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;
	// 결과와 함게 전달되는 정보 메시지
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	// 확인 기능
	static FItemAddResult AddedNone(const FText& ErrorText)	// 얻고자하는 아이템이 없을 때 : 인벤토리가 꽉 찼거나, 다른 문제 발생. 즉 0을 반환할 때
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;

		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)	//일부만 획득했을 때
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;

		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)	// 모두 획득했을 때
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;

		return AddedAllResult;
	};
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ETHERIA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Functions
	/// </summary>
	UInventoryComponent();

	// Change Inventory Type
	UFUNCTION(Category = "Inventory")
	void ChangeInventoryType(EItemType type);

	// 아이템 추가를 위한 함수.
	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	// 해당 아이템 포인터가 인벤토리에 존재하는지 및 동일한지 체크하기 위한 함수.
	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	// ID로 아이템을 찾기 위한 함수.
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* ItemIn) const;
	// 여분 공간 체크
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	// 인벤토리에서 아이템을 삭제.
	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);
	// 떨어트리고자하는 개수만큼 감소 및 삭제.
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);
	// 아이템 분할.
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	// Getters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };	// 인벤토리 무게 반환
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };	// 인벤토리 용량 반환
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; };	// 인벤토리 개수
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents(EItemType type) const {
		switch (type)
		{
		case EItemType::Quest:
			return QuestInventoryContents;
		case EItemType::Consumable:
			return ConsumeInventoryContents;
		case EItemType::Ingredient:
			return IngredientInventoryContents;
		case EItemType::Weapon:
			return WeaponInventoryContents;
		default:
			// 기본값으로 빈 배열을 반환하거나, 로그를 남길 수 있습니다.
			UE_LOG(LogTemp, Warning, TEXT("Unknown item type requested: %d"), static_cast<int32>(type));
			return TArray<UItemBase*>();
		}
	};	// 인벤토리 정보 배열 (아이템 모음)
	FORCEINLINE int32 GetInventoryContentsNum() const {
		return QuestInventoryContents.Num() + ConsumeInventoryContents.Num() + IngredientInventoryContents.Num() + WeaponInventoryContents.Num();
	}

	// Setters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };	// 인벤토리 업그레이드시 확장 및 축소 함수.
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };	// 인벤토리 용량 세팅

	/// <summary>
	/// Variables
	/// </summary>
	FOnInventoryUpdated OnInventoryUpdated; // 델리게이트 추가. 우리는 HandleAddItem을 호출해서 AddNewItem함수로 아이템을 추가할 것이다. 그럼 인벤토리 UI도 업데이트가 되어야 하기 위해서 추가함.

	UPROPERTY(VisibleAnyWhere, Category = "Inventory")
	EItemType CurrentInventoryType;

protected:
	/// <summary>
	/// Functions
	/// </summary>
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* InputItem, int32 RequestedAddAmount);
	int32 HandleStackableItems(UItemBase* InputItem, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	// 실제 추가하는 인벤토리에 아이템을 함수.
	void AddNewItem(UItemBase* Item, const int32 AmountToAdd);

	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> QuestInventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> ConsumeInventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> IngredientInventoryContents;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> WeaponInventoryContents;
};
