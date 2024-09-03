// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponWheelComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponWheelUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETHERIA_API UWeaponWheelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponWheelComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "WeaponWheel")
	FRotator GetMouseRotation();
};
