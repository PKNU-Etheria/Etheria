// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponWheelComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UWeaponWheelComponent::UWeaponWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

FRotator UWeaponWheelComponent::GetMouseRotation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return FRotator::ZeroRotator;

	float MouseX, MouseY;
	bool bGotMousePosition = PlayerController->GetMousePosition(MouseX, MouseY);
	if (!bGotMousePosition)  return FRotator::ZeroRotator;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D ScreenCenter = ViewportSize * 0.5f;

	FVector MousePositionVector(MouseX, MouseY, 0.0f);

	FVector ScreenCenterVector(ScreenCenter.X, ScreenCenter.Y, 0.0f);

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ScreenCenterVector, MousePositionVector);

	LookAtRotation.Yaw = 180.0f - LookAtRotation.Yaw;

	return LookAtRotation;
}

