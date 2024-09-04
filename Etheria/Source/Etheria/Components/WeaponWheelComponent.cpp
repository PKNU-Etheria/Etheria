// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponWheelComponent.h"

// Sets default values for this component's properties
UWeaponWheelComponent::UWeaponWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurSectionAngle = 0;
	SectionCount = 4;
	SectionSize = 360.0f / SectionCount;

	if (UTexture2D* BowImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/bow.bow'")))
	{
		WeaponSectionDefaultImages.Add(BowImage);
	}
	if (UTexture2D* GauntletImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/gauntlet.gauntlet'")))
	{
		WeaponSectionDefaultImages.Add(GauntletImage);
	}
	if (UTexture2D* SpearImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/spear.spear'")))
	{
		WeaponSectionDefaultImages.Add(SpearImage);
	}
	if (UTexture2D* SwordImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Item/Textures/Weapon/sword.sword'")))
	{
		WeaponSectionDefaultImages.Add(SwordImage);
	}

	// ...
}


// Called when the game starts
void UWeaponWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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

