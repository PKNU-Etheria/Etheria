// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEPlayerController();

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
};
