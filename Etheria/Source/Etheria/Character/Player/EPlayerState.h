// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "EPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AEPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AEPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	//ASC
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

};
