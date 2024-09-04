// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWheel.generated.h"

class UWeaponWheelComponent;
class AEPlayer;

UCLASS()
class ETHERIA_API UWeaponWheel : public UUserWidget
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Functions
	/// </summary>
	void CalculateSectionBounds();

	void CalculateSection();

	void CheckSection();

	void UpdateActiveSection();	// Update MI_WeaponWheel Globar Scalar Parameter Value - ActiveRotation Update

	UFUNCTION(BlueprintCallable, Category = "WeaponWheel")
	FRotator GetMouseRotation();
	
	/// <summary>
	/// Variables
	/// </summary>
	UPROPERTY()
	AEPlayer* PlayerCharacter;

	UPROPERTY()
	UWeaponWheelComponent* WeaponWheelReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponWheel", meta = (AllowPrivateAccess = "true"))
	int32 SelectedAngle;

protected:
	/// <summary>
	/// Functions
	/// </summary>
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	/// <summary>
	/// Variables
	/// </summary>
	// 동적 머티리얼 인스턴스
	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* RadialMenuMat;

	// WeaponWheel을 표현하는 이미지 (UI에서 설정된 경우)
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponWheel;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel")
	TArray<int32> MaxBounds;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponWheel")
	TArray<int32> MinBounds;
};
