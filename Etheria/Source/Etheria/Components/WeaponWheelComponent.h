// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponWheelComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponWheelWidgetUpdated);	// Update WeaponWheel Widget

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
	FRotator GetMouseRotation();

	UFUNCTION(BlueprintCallable, Category = "WeaponWheel")
	void ChangeCurSectionValue(int newVal);

	/// <summary>
	/// Variables
	/// </summary>
	FOnWeaponWheelWidgetUpdated OnWeaponWheelWidgetUpdated;	// add update WeaponWheelWidget(WeaponWheel)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponWheel", meta = (AllowPrivateAccess = "true"))
	int32 CurSectionAngle;	// Player Setting WeaponWheel Section index(Weapon)

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
