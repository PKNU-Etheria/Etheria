// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponWheelComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponWheelWidgetUpdated);	// Update WeaponWheel Widget

class UItemBase;

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
	void ChangeCurSectionValue(int newVal);

	FORCEINLINE int32 GetSectionCount() const { return SectionCount; };
	FORCEINLINE float GetSectionSize() const { return SectionSize; };

	UFUNCTION(Category = "WeaponWheel")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return WeaponWheelContents; };

	/// <summary>
	/// Variables
	/// </summary>
	FOnWeaponWheelWidgetUpdated OnWeaponWheelWidgetUpdated;	// add update WeaponWheelWidget(WeaponWheel)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponWheel", meta = (AllowPrivateAccess = "true"))
	int32 CurSectionAngle;	// Player Setting WeaponWheel Section index(Weapon)

	TArray<UTexture2D*> WeaponSectionDefaultImages;	// 0: Bow, 1: Gauntlet, 2: Spear, 3: Sword

protected:
	/// <summary>
	/// Functions
	/// </summary>
	// Called when the game starts
	virtual void BeginPlay() override;

	/// <summary>
	/// Variables
	/// </summary>
	// 섹션 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponWheel")
	int32 SectionCount;

	// 섹션 크기
	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel")
	float SectionSize;

	UPROPERTY(VisibleAnywhere, Category = "WeaponWheel")
	TArray<TObjectPtr<UItemBase>> WeaponWheelContents;
};
