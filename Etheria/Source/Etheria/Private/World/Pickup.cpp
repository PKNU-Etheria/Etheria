// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"
#include "Character/Player/EPlayer.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractComponent.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{	// 초기 세팅
	if (ItemDataTable && !DesiredItemID.IsNone())
	{	// 픽업할 아이템 데이터 정보들을 로드해서 업데이트함.
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		ItemReference->NumericData.bIsStackable = (ItemData->NumericData.MaxStackSize > 1);
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{	// 드롭된 아이템을 획득하기 위한 세팅. 플레이어가 줍고 -> 인벤토리에서 업데이트할 데이터 정보를 세팅.
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OwningInventory = nullptr;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);	// 메쉬의 경계에 접근 허용
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);	// 메쉬의 경계에 접근 허용
	}
}

void APickup::Interact(ACharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const ACharacter* Taker)
{
	if (!IsPendingKillPending())
	{	// 해당 액터가 파괴되기 시작할 때! 즉 플레이어가 아이템을 주웠을 때
		UE_LOG(LogTemp, Warning, TEXT("Let's TakePickUp Test"));
		if (ItemReference)
		{	// 아이템 정보가 유효하면
			const AEPlayer* TPlayer = Cast<AEPlayer>(Taker);
			UE_LOG(LogTemp, Warning, TEXT("Player Clear"));
			if (UInventoryComponent* PlayerInventory = TPlayer->GetInventory())
			{	// 플레이어에 부착되어 있는 플레이어 인벤토리 컴포넌트를 체크.
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
				UE_LOG(LogTemp, Warning, TEXT("Get Inventory"));
				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					UE_LOG(LogTemp, Warning, TEXT("NoItemAdded Test"));
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					UE_LOG(LogTemp, Warning, TEXT("AmountItemAdded Test"));
					TPlayer->GetInteractComponent()->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					UE_LOG(LogTemp, Warning, TEXT("AllItemAdded Test"));
					Destroy();
					break;
				default:
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null!"));
			}

			// 플레이어 인벤토리에 아이템을 추가
			// 추가 연산의 결과를 통해.
			// 아이템을 파괴하거나 조정함.
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
		}
	}
}

void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
	{
		if (ItemDataTable)
		{
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}

