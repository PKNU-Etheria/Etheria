// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponWheelComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponWheelWidgetUpdated);	// Update WeaponWheel Widget
DECLARE_MULTICAST_DELEGATE(FOnWeaponWheelUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETHERIA_API UWeaponWheelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/// <summary>
	/// Functions
	/// </summary>
	// Sets default values for this component's properties
	UWeaponWheelComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "WeaponWheel")
	void ChangeCurSectionValue(int newVal);	// �����ٿ��� ���� ����Ǹ� -> �÷��̾� ���� ���� üũ

	UFUNCTION(BlueprintCallable, Category = "WeaponWheel")
	void ChangeWeaponWheelSectionItem(int SlotIndex, UItemBase* Item = nullptr);	// �κ��丮 �����ٿ��� ���� ����Ǹ� -> �� ������ ������Ʈ�� ���빰�� ������Ʈ �Ǹ��� ������Ʈ (�÷��̾� ��������, ������ �̹���, �̹��� ������ �̹���)

	FORCEINLINE int32 GetSectionCount() const { return SectionCount; };
	FORCEINLINE float GetSectionSize() const { return SectionSize; };

	UFUNCTION(Category = "WeaponWheel")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return WeaponWheelContents; };

	/// <summary>
	/// Variables
	/// </summary>
	FOnWeaponWheelWidgetUpdated OnWeaponWheelWidgetUpdated;	// add update WeaponWheelWidget(WeaponWheel) -> ActiveSection
	FOnWeaponWheelUpdated OnWeaponWheelUpdated;	// Refresh Data Update related on WeaponWheel

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponWheel", meta = (AllowPrivateAccess = "true"))
	int32 CurSectionAngle;	// Player Setting WeaponWheel Section index(Weapon)

	TArray<class UItemBase*> WeaponSectionDefaultData;	// 0: Sword, 1: Spear, 2: Gauntlet , 3: Cane

protected:
	/// <summary>
	/// Functions
	/// </summary>
	// Called when the game starts
	virtual void BeginPlay() override;

	void ChangeWeapon(UItemBase* Item);

	/// <summary>
	/// Variables
	/// </summary>
	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponWheel")
	int32 SectionCount;

	// ���� ũ��
	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel")
	float SectionSize;

	UPROPERTY(VisibleAnywhere, Category = "WeaponWheel")
	TArray<TObjectPtr<class UItemBase>> WeaponWheelContents;
};
