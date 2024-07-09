// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/EPlayerController.h"

AEPlayerController::AEPlayerController()
{

}

void AEPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AEPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}
