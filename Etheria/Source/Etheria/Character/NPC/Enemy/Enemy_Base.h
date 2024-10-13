// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Etheria/Character/ECharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Enemy_Base.generated.h"

/**
 * 
 */
UCLASS()
class ETHERIA_API AEnemy_Base : public AECharacter
{
	GENERATED_BODY()

public:
	AEnemy_Base();

	// AI
	void SetupStimulus();

public:
	// AI
	class UAIPerceptionStimuliSourceComponent* Stimulus;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;
};
