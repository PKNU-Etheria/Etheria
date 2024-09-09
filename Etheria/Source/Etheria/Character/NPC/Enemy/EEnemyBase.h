// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ECharacter.h"
#include "EEnemyBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FBossCleared)

UCLASS()
class ETHERIA_API AEEnemyBase : public AECharacter
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	void BossCleared();

	FBossCleared Delegate_BossCleared;
};
