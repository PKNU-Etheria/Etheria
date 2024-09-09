// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CinematicTestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API ACinematicTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void BossCleared();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayBossClearCinematic();

	UPROPERTY(EditAnywhere)
	class UDataTable* ItemDB;

	UPROPERTY(EditAnywhere)
	FName RewardItemID;
	
};
