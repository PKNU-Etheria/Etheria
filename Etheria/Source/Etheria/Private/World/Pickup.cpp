// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"

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
{	// �ʱ� ����
	if (ItemDataTable && !DesiredItemID.IsNone())
	{	// �Ⱦ��� ������ ������ �������� �ε��ؼ� ������Ʈ��.
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{	// ��ӵ� �������� ȹ���ϱ� ���� ����. �÷��̾ �ݰ� -> �κ��丮���� ������Ʈ�� ������ ������ ����.
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
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
		PickupMesh->SetRenderCustomDepth(true);	// �޽��� ��迡 ���� ���
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);	// �޽��� ��迡 ���� ���
	}
}

void APickup::Interact(AItemTestCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const AItemTestCharacter* Taker)
{
	if (IsPendingKillPending())
	{	// �ش� ���Ͱ� �ı��Ǳ� ������ ��! �� �÷��̾ �������� �ֿ��� ��
		if (ItemReference)
		{	// ������ ������ ��ȿ�ϸ�
			//if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			//{	// �÷��̾ �����Ǿ� �ִ� �÷��̾� �κ��丮 ������Ʈ�� üũ.

			//}

			// �÷��̾� �κ��丮�� �������� �߰�
			// �߰� ������ ����� ����.
			// �������� �ı��ϰų� ������.
		}
	}
}

