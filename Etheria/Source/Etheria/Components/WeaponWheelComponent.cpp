// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WeaponWheelComponent.h"
#include "EtheriaStructs.h"
#include "Item/Item/ItemBase.h"

// Sets default values for this component's properties
UWeaponWheelComponent::UWeaponWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurSectionAngle = 0;
	SectionCount = 4;
	SectionSize = 360.0f / SectionCount;

	// ...
}


// Called when the game starts
void UWeaponWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	if (UTexture2D* SwordImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/sword.sword'")))
	{
		UItemBase* SwordItemBase = NewObject<UItemBase>(this, UItemBase::StaticClass());
		SwordItemBase->AssetData.Icon = SwordImage;

		WeaponSectionDefaultData.Add(SwordItemBase);
	}
	if (UTexture2D* SpearImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/spear.spear'")))
	{
		UItemBase* SpearItemBase = NewObject<UItemBase>(this, UItemBase::StaticClass());
		SpearItemBase->AssetData.Icon = SpearImage;

		WeaponSectionDefaultData.Add(SpearItemBase);
	}
	if (UTexture2D* GauntletImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/gauntlet.gauntlet'")))
	{
		UItemBase* GauntletItemBase = NewObject<UItemBase>(this, UItemBase::StaticClass());
		GauntletItemBase->AssetData.Icon = GauntletImage;

		WeaponSectionDefaultData.Add(GauntletItemBase);
	}
	if (UTexture2D* CaneImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/bow.bow'")))
	{
		UItemBase* CaneItemBase = NewObject<UItemBase>(this, UItemBase::StaticClass());
		CaneItemBase->AssetData.Icon = CaneImage;

		WeaponSectionDefaultData.Add(CaneItemBase);
	}

	for (auto item : WeaponSectionDefaultData)
	{
		WeaponWheelContents.Add(item);
	}

	UE_LOG(LogTemp, Warning, TEXT("Item Count : %d %d"), WeaponSectionDefaultData.Num(), WeaponWheelContents.Num());

	OnWeaponWheelWidgetUpdated.Broadcast();
	OnWeaponWheelUpdated.Broadcast();
}


// Called every frame
void UWeaponWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, GetMouseRotation().ToString());
	// ...
}

void UWeaponWheelComponent::ChangeCurSectionValue(int newVal)
{
	CurSectionAngle = newVal;
	OnWeaponWheelWidgetUpdated.Broadcast();
}

void UWeaponWheelComponent::ChangeWeaponWheelSectionItem(int SlotIndex, UItemBase* Item)
{
}

