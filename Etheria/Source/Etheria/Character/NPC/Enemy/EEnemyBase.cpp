// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/Enemy/EEnemyBase.h"
#include "GameMode/CinematicTestGameMode.h"
#include "Kismet/GameplayStatics.h"

void AEEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	ACinematicTestGameMode* gameMode = Cast<ACinematicTestGameMode>
										(UGameplayStatics::GetGameMode(this));
	if(gameMode)
	{
		Delegate_BossCleared.AddUObject(gameMode, &ACinematicTestGameMode::BossCleared);
	}
}

void AEEnemyBase::BossCleared()
{
	Delegate_BossCleared.Broadcast();
}
